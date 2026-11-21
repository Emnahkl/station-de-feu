#ifndef GIFWRITER_H
#define GIFWRITER_H

// Encodeur GIF89a anime minimal (Qt ne sait pas ecrire de GIF).
// - palette fixe de 256 couleurs (6 rouges x 7 verts x 6 bleus + 4 gris)
// - tramage ordonne (Bayer 4x4) pour lisser les degrades
// - compression LZW standard, boucle infinie (extension NETSCAPE2.0)
//
// Utilisation :
//   GifWriter gif;
//   gif.ouvrir("affiche.gif", 450, 600);
//   gif.ajouterImage(image, 8);   // 8 centiemes de seconde
//   gif.fermer();

#include <QFile>
#include <QString>
#include <cstdint>
#include <vector>

class QImage;

class GifWriter
{
public:
    GifWriter() = default;
    ~GifWriter();

    bool ouvrir(const QString &chemin, int largeur, int hauteur);
    bool ajouterImage(const QImage &image, int delaiCentiemes);
    bool fermer();
    QString erreur() const { return m_erreur; }

    // Exposees pour les tests : compression LZW et quantification d'un pixel
    static std::vector<uint8_t> compresserLZW(const std::vector<uint8_t> &indices, int tailleMinCode = 8);
    static uint8_t indexCouleur(int r, int g, int b, int x, int y);
    static void couleurPalette(int index, uint8_t &r, uint8_t &g, uint8_t &b);

private:
    void ecrire(const void *donnees, int taille);
    void ecrireOctet(uint8_t v);
    void ecrireMot(uint16_t v);

    QFile m_fichier;
    int m_largeur = 0;
    int m_hauteur = 0;
    bool m_ouvert = false;
    QString m_erreur;
};

#endif // GIFWRITER_H
