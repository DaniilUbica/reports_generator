#import <MapKit/MapKit.h>

#ifdef __cplusplus
class MapViewImpl;
#endif

@interface MapViewDelegateBridge : NSObject<MKMapViewDelegate>
@property (assign) MapViewImpl* mapView;
@end
