#ifndef VISUALIZERMODEL_H
#define VISUALIZERMODEL_H

#include <QAbstractProxyModel>

class VisualizerModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit VisualizerModel(QObject *parent = 0);
    
signals:


public slots:
    
};

#endif // VISUALIZERMODEL_H
