#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>

int
main ()
{
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr plane_seg_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PCDReader cloud_reader;
  pcl::PCDWriter cloud_writer;
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);

  std::string path="/home/iron_x/iron_x/src/point_cloud_perception/point_cloud_map/";
  // Reading the cloud
  cloud_reader.read (path+std::string("map.pcd"),*cloud);


//   Planner_segmentation

    pcl::SACSegmentation<pcl::PointXYZ> plane_seg;
    plane_seg.setModelType(pcl::SACMODEL_PLANE);
    plane_seg.setMethodType(pcl::SAC_RANSAC);
    plane_seg.setDistanceThreshold(0.1);
    plane_seg.setInputCloud(cloud);
    plane_seg.segment(*inliers,*coefficients);


// Extracting Points
    pcl::ExtractIndices<pcl::PointXYZ> extract_indicies;
    extract_indicies.setInputCloud(cloud);
    extract_indicies.setIndices(inliers);
    extract_indicies.setNegative(true);
    extract_indicies.filter(*plane_seg_cloud);


  // Write
  cloud_writer.write<pcl::PointXYZ>(path+std::string("planner.pcd"),*plane_seg_cloud, false);


  return (0);
}