    #ifndef ANIMIMATED_IMG_H
    #define ANIMIMATED_IMG_H
    #include <QWidget>
    #include <QString>
    #include <QTimer>
    #include <QLabel>
    #include <QList>
    #include <QPixmap>
     
    class AnimatedImage : public QLabel
    {
    Q_OBJECT
     
    public:
        AnimatedImage(  const QString& image_base, 
                        const int imageCount,
                        const float resize, 
                        QWidget* parent = 0);
     
        void Animate();

    private slots:
        void changeImage();
     
    private:
        QList<QPixmap> pixmaps;
        int currentPixmap;
        QTimer timer;
    };
     
    #endif 