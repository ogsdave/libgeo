#include <iostream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include <opencv2/highgui/highgui.hpp>

#include "dbglog/dbglog.hpp"
#include "geo/geodataset.hpp"

const geo::SrsDefinition utm33
(R"XXX(PROJCS["UTM Zone 33, Northern Hemisphere"
,GEOGCS["WGS 84"
,DATUM["WGS_1984"
,SPHEROID["WGS 84",6378137,298.257223563
,AUTHORITY["EPSG","7030"]]
,TOWGS84[0,0,0,0,0,0,0]
,AUTHORITY["EPSG","6326"]]
,PRIMEM["Greenwich",0,AUTHORITY["EPSG","8901"]]
,UNIT["degree",0.0174532925199433,AUTHORITY["EPSG","9108"]]
,AUTHORITY["EPSG","4326"]]
,PROJECTION["Transverse_Mercator"]
,PARAMETER["latitude_of_origin",0]
,PARAMETER["central_meridian",15]
,PARAMETER["scale_factor",0.9996]
,PARAMETER["false_easting",500000]
,PARAMETER["false_northing",0]
,UNIT["Meter",1]])XXX"
, geo::SrsDefinition::Type::wkt);

BOOST_AUTO_TEST_CASE(geo_generate_dataset)
{
    math::Extents2 extents(586464, 5457440, 590560, 5463584);

    auto ds(geo::GeoDataset::create
            ("./test-gdal-dataset.tif"
             , utm33
             , extents
             , { 1024, 1536 }
             , geo::GeoDataset::Format::gtiffRGBPhoto()
             , 0));

    LOG(info3) << "Original extents: " << std::setprecision(15) << extents;
    LOG(info3) << "Dataset extents: " << std::setprecision(15) << ds.extents();

    BOOST_CHECK_EQUAL(extents, ds.extents());

    // one third of extents
    auto bsize(size(extents) / 3.);

    auto p1(ds.geo2raster<cv::Point>(extents.ll(0) + bsize.width
                                     , extents.ll(1) + bsize.height));

    auto p2(ds.geo2raster<cv::Point>(extents.ur(0) - bsize.width
                                     , extents.ur(1) - bsize.height));


    cv::rectangle(ds.data(), p1, p2, cv::Scalar(255, 192, 128)
                  , CV_FILLED, 4);

    ds.flush();
}