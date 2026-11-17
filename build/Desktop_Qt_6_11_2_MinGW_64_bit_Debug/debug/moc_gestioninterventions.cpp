/****************************************************************************
** Meta object code from reading C++ file 'gestioninterventions.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../gestioninterventions.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gestioninterventions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN20GestionInterventionsE_t {};
} // unnamed namespace

template <> constexpr inline auto GestionInterventions::qt_create_metaobjectdata<qt_meta_tag_ZN20GestionInterventionsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GestionInterventions",
        "rafraichir",
        "",
        "ajouter",
        "modifier",
        "supprimer",
        "consulter",
        "bandeauValider",
        "bandeauAnnuler",
        "enregistrerEdition",
        "annulerEdition",
        "confirmerSuppression",
        "majBoutons",
        "exporterPdf",
        "trierParDate",
        "trierParId",
        "afficherStats",
        "afficherParcours",
        "afficherAffectation",
        "validerAffectation",
        "afficherRessources",
        "fonctionnaliteAVenir"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'rafraichir'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'ajouter'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'modifier'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'supprimer'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'consulter'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'bandeauValider'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'bandeauAnnuler'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'enregistrerEdition'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'annulerEdition'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'confirmerSuppression'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'majBoutons'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'exporterPdf'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'trierParDate'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'trierParId'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'afficherStats'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'afficherParcours'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'afficherAffectation'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'validerAffectation'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'afficherRessources'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'fonctionnaliteAVenir'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GestionInterventions, qt_meta_tag_ZN20GestionInterventionsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GestionInterventions::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestionInterventionsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestionInterventionsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20GestionInterventionsE_t>.metaTypes,
    nullptr
} };

void GestionInterventions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GestionInterventions *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->rafraichir(); break;
        case 1: _t->ajouter(); break;
        case 2: _t->modifier(); break;
        case 3: _t->supprimer(); break;
        case 4: _t->consulter(); break;
        case 5: _t->bandeauValider(); break;
        case 6: _t->bandeauAnnuler(); break;
        case 7: _t->enregistrerEdition(); break;
        case 8: _t->annulerEdition(); break;
        case 9: _t->confirmerSuppression(); break;
        case 10: _t->majBoutons(); break;
        case 11: _t->exporterPdf(); break;
        case 12: _t->trierParDate(); break;
        case 13: _t->trierParId(); break;
        case 14: _t->afficherStats(); break;
        case 15: _t->afficherParcours(); break;
        case 16: _t->afficherAffectation(); break;
        case 17: _t->validerAffectation(); break;
        case 18: _t->afficherRessources(); break;
        case 19: _t->fonctionnaliteAVenir(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *GestionInterventions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestionInterventions::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestionInterventionsE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GestionInterventions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
