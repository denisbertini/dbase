#ifndef FAIRDBWTPAINTEDIMAGE
#define FAIRDBWTPAINTEDIMAGE

#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>

class FairDbWtPaintedImage : public Wt::WPaintedWidget
{
public:
    FairDbWtPaintedImage(Wt::WContainerWidget *parent = 0)
    : Wt::WPaintedWidget(parent)
    {
    setLayoutSizeAware(true);
    resize(1000, 1000);  // Provide a default size.
    }

protected:
    void paintEvent(Wt::WPaintDevice *paintDevice);
    
};

#endif //FAIRDBWTPAINTEDIMAGE
