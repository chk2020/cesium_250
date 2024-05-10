#include "TestRegionPolygons.h"

#include <algorithm>

namespace Cesium {
static const TestPolygon testRegionPolygonsData[] = {
    {FVector2D(-74.2013321117691, 40.9774450853578),
     FVector2D(-74.4593744591054, 40.514167617152),
     FVector2D(-72.9155255739658, 40.7042737041433),
     FVector2D(-73.85290089588, 40.9326611125325),
     FVector2D(-74.2013321117691, 40.9774450853578)},
    {FVector2D(-70.8665457211077, 42.5869988084598),
     FVector2D(-71.3661722404885, 42.4639488342876),
     FVector2D(-71.2972087427719, 42.1467522915001),
     FVector2D(-70.7729743178252, 42.2335714316977),
     FVector2D(-70.8665457211077, 42.5869988084598)},
    {FVector2D(-75.0239947410133, 40.1395284511288),
     FVector2D(-75.4196355358948, 40.0148058322418),
     FVector2D(-75.2636118623868, 39.7886799431864),
     FVector2D(-74.7858557650542, 39.9438636936503),
     FVector2D(-75.0239947410133, 40.1395284511288)},
    {FVector2D(-76.7463533262082, 39.4825406033305),
     FVector2D(-77.4340311059877, 38.8258770447382),
     FVector2D(-76.9547073596801, 38.6332198167468),
     FVector2D(-76.2326804229044, 39.3697178750327),
     FVector2D(-76.7463533262082, 39.4825406033305)},
    {FVector2D(-87.7880477582706, 42.484868555962),
     FVector2D(-88.5878253665254, 42.3173994498162),
     FVector2D(-88.2041661838266, 41.2937330828566),
     FVector2D(-87.2835529091586, 41.5464660939422),
     FVector2D(-87.7880477582706, 42.484868555962)},
    {FVector2D(-117.797555097187, 33.5210842049415),
     FVector2D(-116.732710054453, 34.1503747429439),
     FVector2D(-118.101396808054, 34.3250274422573),
     FVector2D(-119.005372293813, 34.0462414684101),
     FVector2D(-117.797555097187, 33.5210842049415)},
    {FVector2D(-121.952555819749, 37.0551315779953),
     FVector2D(-121.472262225148, 37.2762707697842),
     FVector2D(-122.308508157973, 38.2801293998287),
     FVector2D(-122.735942423996, 37.8231990412902),
     FVector2D(-121.952555819749, 37.0551315779953)},
    {FVector2D(-123.344029747329, 49.3851693056083),
     FVector2D(-123.302612411589, 48.8580301457693),
     FVector2D(-122.05501553595, 48.8358263385625),
     FVector2D(-121.933325112706, 49.2715130224448),
     FVector2D(-123.344029747329, 49.3851693056083)},
    {FVector2D(-98.894461647111, 19.1607853097298),
     FVector2D(-98.7686431463364, 19.6183748878263),
     FVector2D(-99.3048055915005, 19.6853872937311),
     FVector2D(-99.3991868765643, 19.3265084381959),
     FVector2D(-98.894461647111, 19.1607853097298)},
    {FVector2D(140.054350788689, 35.0230502784051),
     FVector2D(140.412218892413, 36.3913714622335),
     FVector2D(139.256616929582, 36.4982427020207),
     FVector2D(139.10640870208, 34.9240606249587),
     FVector2D(140.054350788689, 35.0230502784051)},
    {FVector2D(135.055269955952, 34.1165959544619),
     FVector2D(136.010060967071, 34.3342346904423),
     FVector2D(135.827593998297, 35.1609921898404),
     FVector2D(134.951100611567, 34.7581621821003),
     FVector2D(135.055269955952, 34.1165959544619)},
    {FVector2D(126.506482128111, 37.309652936223),
     FVector2D(127.276578372058, 37.3152753352917),
     FVector2D(127.276625945403, 37.7169510199583),
     FVector2D(126.456565879908, 37.6708092887235),
     FVector2D(126.506482128111, 37.309652936223)},
    {FVector2D(121.134001967184, 31.5497391098806),
     FVector2D(121.162908585755, 30.9008622819707),
     FVector2D(121.973194206992, 30.996871119183),
     FVector2D(121.703379279597, 31.5459147489145),
     FVector2D(121.134001967184, 31.5497391098806)},
    {FVector2D(113.118683475979, 21.9538081632891),
     FVector2D(113.116571761854, 21.9536183257244),
     FVector2D(114.51719761199, 22.1592194597545),
     FVector2D(114.412643287974, 23.2156098976288),
     FVector2D(112.968966605958, 23.1794019008474),
     FVector2D(113.118683475979, 21.9538081632891)},
    {FVector2D(119.602455828861, 23.3701241133722),
     FVector2D(120.43065363132, 22.7299422907066),
     FVector2D(121.979122588007, 24.9890484436014),
     FVector2D(121.331012296727, 25.3178637634457),
     FVector2D(119.602455828861, 23.3701241133722)},
    {FVector2D(116.388332170902, 38.6463257232499),
     FVector2D(118.085699455998, 38.898921209556),
     FVector2D(117.787877280437, 40.1604047317278),
     FVector2D(115.944808581142, 40.0167139637757),
     FVector2D(116.388332170902, 38.6463257232499)},
    {FVector2D(120.760414545729, 14.3748010156788),
     FVector2D(121.258394771143, 14.2501216390828),
     FVector2D(121.290054862448, 14.7534041084167),
     FVector2D(120.738158949159, 14.7593332219211),
     FVector2D(120.760414545729, 14.3748010156788)},
    {FVector2D(103.50156107247, 0.998061113465564),
     FVector2D(104.715513672489, 0.982204662790223),
     FVector2D(104.334518818725, 1.70398117696512),
     FVector2D(103.258944549305, 1.4844867069418),
     FVector2D(103.50156107247, 0.998061113465564)},
    {FVector2D(105.881719721888, -5.74570400497445),
     FVector2D(105.833559417072, -6.21278484163883),
     FVector2D(108.186160253085, -6.47215807929743),
     FVector2D(108.027660861491, -5.93654743166617),
     FVector2D(105.881719721888, -5.74570400497445)},
    {FVector2D(150.583157022031, -34.0498630944677),
     FVector2D(151.280108788925, -34.1723736264387),
     FVector2D(151.430931749967, -33.6913578323042),
     FVector2D(150.68777700301, -33.5004609703255),
     FVector2D(150.583157022031, -34.0498630944677)},
    {FVector2D(100.362043393093, 13.5146830907152),
     FVector2D(100.804723956777, 13.4902538260098),
     FVector2D(100.857533774329, 14.0291484359391),
     FVector2D(100.296514387822, 14.0180095766592),
     FVector2D(100.362043393093, 13.5146830907152)},
    {FVector2D(72.8139207040816, 18.8644328953372),
     FVector2D(73.0797035981117, 18.9090108585454),
     FVector2D(73.0550830148766, 19.2511180020347),
     FVector2D(72.765539257819, 19.2056123110889),
     FVector2D(72.8139207040816, 18.8644328953372)},
    {FVector2D(76.9327816012394, 28.3841052539538),
     FVector2D(77.4771720538251, 28.4833789366456),
     FVector2D(77.3482561523984, 28.8846883488663),
     FVector2D(76.8582384578204, 28.7963756494536),
     FVector2D(76.9327816012394, 28.3841052539538)},
    {FVector2D(54.9429835012101, 25.0178517226697),
     FVector2D(55.0946994816812, 24.8756220574815),
     FVector2D(55.602636800682, 25.3172895563037),
     FVector2D(55.3945349608567, 25.4562123460321),
     FVector2D(54.9429835012101, 25.0178517226697)},
    {FVector2D(-3.61878369798117, 40.2775903261193),
     FVector2D(-3.4906994656486, 40.4737663140241),
     FVector2D(-3.72686800427542, 40.601926645219),
     FVector2D(-3.89898175894152, 40.3290711912317),
     FVector2D(-3.61878369798117, 40.2775903261193)},
    {FVector2D(-9.19296732462638, 38.3938592975828),
     FVector2D(-8.75078735276178, 38.5702884330086),
     FVector2D(-8.92050164224059, 38.9449362396636),
     FVector2D(-9.5272031704166, 38.7658866470946),
     FVector2D(-9.19296732462638, 38.3938592975828)},
    {FVector2D(1.81826634445193, 48.6429184198264),
     FVector2D(2.57743142898819, 48.5161116317191),
     FVector2D(2.81852244248245, 48.9958850870085),
     FVector2D(2.08220061664221, 49.1112351713166),
     FVector2D(1.81826634445193, 48.6429184198264)},
    {FVector2D(12.046678722878, 41.8734309143794),
     FVector2D(12.538572569152, 41.5584390046116),
     FVector2D(12.902473689969, 41.8844741109742),
     FVector2D(12.3604328937937, 42.1775684704485),
     FVector2D(12.046678722878, 41.8734309143794)},
    {FVector2D(13.0369523148704, 52.3264792264681),
     FVector2D(13.7857078301483, 52.2432874392554),
     FVector2D(13.7245722259762, 52.5706652767158),
     FVector2D(13.0855894510726, 52.6398386596651),
     FVector2D(13.0369523148704, 52.3264792264681)},
    {FVector2D(-0.540173561477041, 51.3275558776523),
     FVector2D(0.366937281698374, 51.2972794865316),
     FVector2D(0.378630660590049, 51.6312453736339),
     FVector2D(-0.565434346898922, 51.6941551135231),
     FVector2D(-0.540173561477041, 51.3275558776523)},
    {FVector2D(-3.44162716057316, 55.9042055474235),
     FVector2D(-2.86557475273948, 55.8204519638228),
     FVector2D(-2.76434520027445, 55.9879349630359),
     FVector2D(-3.39396844082001, 56.0516294637465),
     FVector2D(-3.44162716057316, 55.9042055474235)},
    {FVector2D(-6.45226364658212, 53.233148089773),
     FVector2D(-5.94733626257021, 53.2160280458379),
     FVector2D(-5.99718303046307, 53.4420617215498),
     FVector2D(-6.53307425159679, 53.4371162576022),
     FVector2D(-6.45226364658212, 53.233148089773)}};

const TestPolygon* TestRegionPolygons::polygons = testRegionPolygonsData;

FVector TestPolygon::GetRandomPoint() const {
  // For testing we don't need to actually generate a random point within the
  // polygon - just use the polygon's bounding box.
  double minX = std::numeric_limits<double>::max();
  double minY = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::min();
  double maxY = std::numeric_limits<double>::min();

  for (const FVector2D& point : this->points) {
    minX = std::min(point.X, minX);
    minY = std::min(point.Y, minY);
    maxX = std::max(point.X, maxX);
    maxY = std::max(point.Y, maxY);
  }

  return FVector(
      FMath::RandRange(minX, maxX),
      FMath::RandRange(minY, maxY),
      0.0f);
}

} // namespace Cesium
