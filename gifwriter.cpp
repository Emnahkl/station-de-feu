#include "gifwriter.h"

#include <QImage>
#include <unordered_map>

namespace {

// Niveaux de la palette fixe : 6 x 7 x 6 = 252 couleurs + 4 gris = 256
const int NR = 6, NG = 7, NB = 6;

inline int niveau(int v, int n) { return (v * (n - 1) + 127) / 255; }
inline int valeur(int k, int n) { return (k * 255) / (n - 1); }

// Matrice de Bayer 4x4 (seuils de -7.5 a +7.5 en 16e de pas)
const int BAYER[4][4] = { { 0, 8, 2, 10 }, { 12, 4, 14, 6 }, { 3, 11, 1, 9 }, { 15, 7, 13, 5 } };

// Ecrit des codes de longueur variable, poids faible d'abord (format GIF)
class Bits
{
public:
    explicit Bits(std::vector<uint8_t> &out) : m_out(out) {}
    void ecrire(int code, int nbBits)
    {
        m_acc |= static_cast<uint32_t>(code) << m_nb;
        m_nb += nbBits;
        while (m_nb >= 8) {
            m_out.push_back(static_cast<uint8_t>(m_acc & 0xFF));
            m_acc >>= 8;
            m_nb -= 8;
        }
    }
    void vider()
    {
        if (m_nb > 0)
            m_out.push_back(static_cast<uint8_t>(m_acc & 0xFF));
        m_acc = 0;
        m_nb = 0;
    }

private:
    std::vector<uint8_t> &m_out;
    uint32_t m_acc = 0;
    int m_nb = 0;
};

} // namespace

GifWriter::~GifWriter()
{
    if (m_ouvert)
        fermer();
}

void GifWriter::couleurPalette(int index, uint8_t &r, uint8_t &g, uint8_t &b)
{
    if (index < NR * NG * NB) {
        r = static_cast<uint8_t>(valeur(index / (NG * NB), NR));
        g = static_cast<uint8_t>(valeur((index / NB) % NG, NG));
        b = static_cast<uint8_t>(valeur(index % NB, NB));
    } else {
        // 4 gris intermediaires
        const uint8_t gris[4] = { 32, 96, 160, 224 };
        r = g = b = gris[index - NR * NG * NB];
    }
}

uint8_t GifWriter::indexCouleur(int r, int g, int b, int x, int y)
{
    // Tramage : decalage proportionnel a l'ecart entre deux niveaux de la palette
    const int seuil = BAYER[y & 3][x & 3] * 2 - 15;   // -15 .. +15
    auto tramer = [seuil](int v, int n) {
        const int pas = 255 / (n - 1);
        return qBound(0, v + seuil * pas / 32, 255);
    };
    const int ir = niveau(tramer(r, NR), NR);
    const int ig = niveau(tramer(g, NG), NG);
    const int ib = niveau(tramer(b, NB), NB);
    return static_cast<uint8_t>(ir * NG * NB + ig * NB + ib);
}

std::vector<uint8_t> GifWriter::compresserLZW(const std::vector<uint8_t> &indices, int tailleMinCode)
{
    std::vector<uint8_t> out;
    Bits bits(out);
    const int codeClear = 1 << tailleMinCode;
    const int codeFin = codeClear + 1;

    std::unordered_map<uint32_t, int> dico;
    dico.reserve(8192);
    int tailleCode = tailleMinCode + 1;
    int prochain = codeFin + 1;

    bits.ecrire(codeClear, tailleCode);
    if (indices.empty()) {
        bits.ecrire(codeFin, tailleCode);
        bits.vider();
        return out;
    }

    int prefixe = indices[0];
    for (size_t i = 1; i < indices.size(); ++i) {
        const int k = indices[i];
        const uint32_t cle = (static_cast<uint32_t>(prefixe) << 8) | static_cast<uint32_t>(k);
        const auto it = dico.find(cle);
        if (it != dico.end()) {
            prefixe = it->second;
            continue;
        }
        bits.ecrire(prefixe, tailleCode);
        if (prochain < 4096) {
            dico.emplace(cle, prochain);
            // Le decodeur agrandit ses codes quand son prochain code atteint 2^taille
            if (prochain == (1 << tailleCode) && tailleCode < 12)
                ++tailleCode;
            ++prochain;
        } else {
            // Table pleine : on la reinitialise
            bits.ecrire(codeClear, tailleCode);
            dico.clear();
            tailleCode = tailleMinCode + 1;
            prochain = codeFin + 1;
        }
        prefixe = k;
    }
    bits.ecrire(prefixe, tailleCode);
    bits.ecrire(codeFin, tailleCode);
    bits.vider();
    return out;
}

void GifWriter::ecrire(const void *donnees, int taille)
{
    m_fichier.write(static_cast<const char *>(donnees), taille);
}

void GifWriter::ecrireOctet(uint8_t v)
{
    m_fichier.putChar(static_cast<char>(v));
}

void GifWriter::ecrireMot(uint16_t v)
{
    ecrireOctet(static_cast<uint8_t>(v & 0xFF));
    ecrireOctet(static_cast<uint8_t>(v >> 8));
}

bool GifWriter::ouvrir(const QString &chemin, int largeur, int hauteur)
{
    m_fichier.setFileName(chemin);
    if (!m_fichier.open(QIODevice::WriteOnly)) {
        m_erreur = m_fichier.errorString();
        return false;
    }
    m_largeur = largeur;
    m_hauteur = hauteur;
    m_ouvert = true;

    ecrire("GIF89a", 6);
    ecrireMot(static_cast<uint16_t>(largeur));
    ecrireMot(static_cast<uint16_t>(hauteur));
    ecrireOctet(0xF7);   // palette globale de 256 couleurs (8 bits)
    ecrireOctet(0);      // couleur de fond
    ecrireOctet(0);      // rapport d'aspect
    for (int i = 0; i < 256; ++i) {
        uint8_t r, g, b;
        couleurPalette(i, r, g, b);
        ecrireOctet(r);
        ecrireOctet(g);
        ecrireOctet(b);
    }

    // Boucle infinie
    ecrireOctet(0x21);
    ecrireOctet(0xFF);
    ecrireOctet(11);
    ecrire("NETSCAPE2.0", 11);
    ecrireOctet(3);
    ecrireOctet(1);
    ecrireMot(0);
    ecrireOctet(0);
    return true;
}

bool GifWriter::ajouterImage(const QImage &image, int delaiCentiemes)
{
    if (!m_ouvert)
        return false;

    const QImage img = image.convertToFormat(QImage::Format_RGB32)
                           .scaled(m_largeur, m_hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    std::vector<uint8_t> indices(static_cast<size_t>(m_largeur) * m_hauteur);
    for (int y = 0; y < m_hauteur; ++y) {
        const QRgb *ligne = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        for (int x = 0; x < m_largeur; ++x)
            indices[static_cast<size_t>(y) * m_largeur + x]
                = indexCouleur(qRed(ligne[x]), qGreen(ligne[x]), qBlue(ligne[x]), x, y);
    }

    // Extension de controle graphique : delai entre images
    ecrireOctet(0x21);
    ecrireOctet(0xF9);
    ecrireOctet(4);
    ecrireOctet(0x04);   // disposition : laisser en place
    ecrireMot(static_cast<uint16_t>(qMax(2, delaiCentiemes)));
    ecrireOctet(0);
    ecrireOctet(0);

    // Descripteur d'image (utilise la palette globale)
    ecrireOctet(0x2C);
    ecrireMot(0);
    ecrireMot(0);
    ecrireMot(static_cast<uint16_t>(m_largeur));
    ecrireMot(static_cast<uint16_t>(m_hauteur));
    ecrireOctet(0);

    // Donnees LZW par sous-blocs de 255 octets
    ecrireOctet(8);
    const std::vector<uint8_t> lzw = compresserLZW(indices, 8);
    size_t pos = 0;
    while (pos < lzw.size()) {
        const int n = static_cast<int>(qMin<size_t>(255, lzw.size() - pos));
        ecrireOctet(static_cast<uint8_t>(n));
        ecrire(lzw.data() + pos, n);
        pos += static_cast<size_t>(n);
    }
    ecrireOctet(0);
    return m_fichier.error() == QFileDevice::NoError;
}

bool GifWriter::fermer()
{
    if (!m_ouvert)
        return false;
    ecrireOctet(0x3B);
    m_fichier.close();
    m_ouvert = false;
    return true;
}
