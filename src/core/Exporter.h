#pragma once
#include "Zone.h"
#include <QString>
#include <QVector>

// Export PDF et Excel (CSV « ; » avec BOM UTF-8, s'ouvre directement dans Excel)
namespace Exporter {
bool toCsv(const QString& path, const QVector<Zone>& zones);
bool toPdf(const QString& path, const QVector<Zone>& zones, double thresholdMin);
}
