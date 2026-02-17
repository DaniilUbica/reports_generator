#import <MapKit/MapKit.h>

#ifdef __cplusplus
class MapViewImpl;
#endif

@interface MapViewDelegateBridge : NSObject<MKMapViewDelegate>

#ifdef __cplusplus
@property (assign) MapViewImpl* mapView;
#else
@property (assign) void* mapView;
#endif

@end
