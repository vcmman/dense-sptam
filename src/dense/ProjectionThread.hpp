#ifndef __PROJECTIONTHREAD_H
#define __PROJECTIONTHREAD_H

#include <thread>
#include <pcl_ros/point_cloud.h>

#include "Camera.hpp"
#include "DispImageQueue.hpp"
#include "PointCloudQueue.hpp"
#include "FrustumCulling.hpp"

#define MY_MISSING_Z            10000.0
#define PIXEL_DISP_INVALID      -10
#define PIXEL_DISP_CORNER       -11

class Dense;

class ProjectionThread
{
public:

    ProjectionThread(Dense *dense);

    inline void WaitUntilFinished()
    { projectionThread_.join(); }

private:

    Dense *dense_;
    std::thread projectionThread_;
    void compute();

    unsigned calculateValidDisp(const DispRawImagePtr disp_raw_img);
    void filterDisp(const DispRawImagePtr disp_raw_img);
    bool isValidDisparity(const float   disp);
    bool isValidPoint(const cv::Vec3f& pt);

    PointCloudPtr my_generateCloud(DispRawImagePtr disp_raw_img);
    PointCloudPtr generateCloud(const DispRawImagePtr disp_raw_img);

    void cameraToWorld(PointCloudPtr cloud, CameraPose::Ptr current_pos);

    void statisticalFilterCloud(PointCloudPtr cloud, double filter_meanK, double filter_stddev);
    void radiusFilterCloud(PointCloudPtr cloud, double filter_radius, double filter_minneighbours);

    PointCloudPtr doStereoscan(PointCloudPtr last_cloud, DispImagePtr disp_img,
                               FrustumCulling *frustum_left, FrustumCulling *frustum_right,
                               CameraPose::Ptr current_pos, double stereoscan_threshold,
                               unsigned int log_data[]);
};

void downsampleCloud(PointCloudPtr cloud, double voxelLeafSize);

#endif /* __PROJECTIONTHREAD_H */
