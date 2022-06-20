#pragma once
#pragma once
#include <pcl/io/ply_io.h>
#include <iostream>
#include <vector>
#include <pcl/io/pcd_io.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <pcl/io/vtk_io.h>
#include <pcl/io/vtk_lib_io.h>//����vtkpolydataת���ƺ���
#include <pcl/io/obj_io.h>
#include <pcl/filters/uniform_sampling.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/segmentation/region_growing.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/uniform_sampling.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>//ŷʽ����
#include <pcl/ModelCoefficients.h>
#include <pcl/common/transforms.h>
#include <pcl/common/common.h>
#include <pcl/common/common_headers.h>
#include <set>

using namespace std;
//���嵼����ʽ����C���Եķ�ʽ��������ΪC���Է�ʽ���������ֲ���
#define EXTERNC extern "C"
//����dll������ʽ���˴��ǵ���������ǵ�����Ϊdllinport
#define HEAD EXTERNC __declspec(dllexport)
//�������Լ�����˴�ѡ���׼����Լ����Ҳ������c����Լ��
#define CallingConvention __stdcall

/*
���ܣ���pointcloud�ṹ����������洢,������ԭʼ�ķ�ʽ�����潫���ƶ����װ�ˣ��ⷽ��������
*/
void Pc2Array(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, double * out_x, double * out_y, double * out_z);
//�����������������������ĵ�ص���������
int getMaxPointCluster(vector<pcl::PointIndices> cluster);
//����ƽ������л����Ϻ��ƽ����xy�ο������б��
double getAngle(double line1[3], double line2[3]);
//���ڻ�ȡ�������ı���ת������
Eigen::Matrix4f getRotationMatrix(Eigen::Vector3f vector_before, Eigen::Vector3f vector_after);
//��������������Ϊ���ƶ���
void connect_pc(vector<pcl::PointCloud<pcl::PointXYZ>>& input_pc1, pcl::PointCloud<pcl::PointXYZ> & out_cloud);

//�������أ�3�����귨���޳��쳣ֵ�������ǵ��ƣ�����ǵ�������
void stdDev_filter(vector<pcl::PointCloud<pcl::PointXYZ>>& input_cloud, vector<pcl::PointCloud<pcl::PointXYZ>>& out_cloud);

/*
���ܣ�����ply��pcd��ʽ�����ļ������������е�X��Y��Z����洢������double������,��������
param[in] path �����ļ�·��
param[out] out_x ���е��X����
param[out] out_y ���е��Y����
param[out] out_z ���е��Z����
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD int CallingConvention loadFile(char* path, double * out_x, double * out_y, double * out_z);

/*
���ܣ�����ply��ʽ�����ļ����������ƴ���PointCloud������
param[in] path �����ļ�·��
param[out] pc ���ƶ���
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD int CallingConvention loadPlyFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc);

/*
���ܣ�����pcd��ʽ�����ļ����������ƴ���PointCloud������
param[in] path �����ļ�·��
param[out] pc ���ƶ���
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD int CallingConvention loadPcdFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc);

/*
���ܣ�����obj�ļ�
param[in] path �ļ�·��
param[out] pc ���ƶ���
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD int CallingConvention loadObjFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc);


/*
���ܣ�����pcd��ʽ�����ļ�
param[in] path �����ļ�·��
param[out] pc ���ƶ���
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD void CallingConvention savePcdFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc, int binaryMode);

/*
���ܣ�����ply��ʽ�����ļ�
param[in] path �����ļ�·��
param[out] pc ���ƶ���
�ɹ������ļ�����1��ʧ���򷵻�0
*/
HEAD void CallingConvention savePlyFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc, int binaryMode);

/*
���ܣ���stl��ʽ�ļ�תΪ���ƶ���
param[in] path stl��ʽ�ļ�·��
param[out] pc ���ƶ���
*/
HEAD void CallingConvention stl2PointCloud(char* path, pcl::PointCloud<pcl::PointXYZ> * pc);

/*
//TODO ��δʵ�ָù��� 2022620
���ܣ����ƴ�Ϊobj��ʽ
param[in] path �洢·��
param[out] pc ������ƶ���
*/
HEAD void CallingConvention saveObjFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc);

/*
���ܣ��Ե��ƽ��о����²���
param[in] in_pc Ŀ����ƶ����ָ��
param[in] radius �����²����ֱ��ʲ�������ֵԽ������������Խϡ��
param[out] out_pc ������ƶ����ָ��
*/
HEAD void CallingConvention uniformDownSample(pcl::PointCloud<pcl::PointXYZ> * in_pc, double radius,
	pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ��Ե��ƽ���ֱͨ�˲�
param[in] in_pc Ŀ����ƶ����ָ��
param[in] axis_name ѡ�ĸ�����й���
param[in] limit_min �����������Сֵ
param[in] limit_max ������������ֵ
param[in] negative ����Ƿ�����Ϊtrue�������[limit_min,limit_max]֮��ĵ㣬һ��Ϊfalse
param[out] out_pc ������ƶ����ָ��
*/
HEAD void CallingConvention passThroughFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc, char * axis_name,
	                                          float limit_min, float limit_max, int negative,
	                                          pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ�ʹ��ͳ���˲��Ե���ȥ�룬����Ⱥ��ȥ��
param[in] in_x ����������x����
param[in] in_y ����������y����
param[in] in_z ����������z����
param[in] neighbor_num �����������ݵ����Ŀѡȡ���ʵ��ھ���,Ĭ��50
param[in] thresh ���ƽ��������[��-������,�̣�������]֮��ĵ㱻�޳���thresh���Ǧ�
param[out] out_x ���������е��X����
param[out] out_y ���������е��Y����
param[out] out_z ���������е��Z����
*/
HEAD void CallingConvention staFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                  int neighbor_num, float thresh,
	                                  pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ��뾶�˲�������Ⱥ��ȥ��
*/
HEAD void CallingConvention radiusFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                     double radius, int num_thresh,
	                                     pcl::PointCloud<pcl::PointXYZ> * out_pc);


/*
���ܣ���װ�������������ֱ�ӷ��ص������Ľ��
param[in] in_pc Ŀ����ƶ���ָ��
param[in] neighbor_num �����������ݵ����Ŀѡȡ���ʵ��ھ���
param[in] smooth_thresh ��������ֵ
param[in] curva_thresh ������ֵ
param[in] MinClusterSize ��Ϊһ�������С����
param[in] MaxClusterSize ��Ϊһ�������������
����ζ�ŷָ��ĵ�����Ŀ������[MinClusterSize,MaxClusterSize]֮����ܱ����
param[out] out_pc ������ƵĶ���ָ��
*/
HEAD void CallingConvention modifiedGrowRegion(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                           int neighbor_num, float smooth_thresh, float curva_thresh,
	                                           int MinClusterSize, int MaxClusterSize,
	                                           pcl::PointCloud<pcl::PointXYZ> * out_pc);

//ԭʼ����������
HEAD void CallingConvention oriGrowRegion(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                      int neighbor_num, float smooth_thresh, float curva_thresh,
	                                      int MinClusterSize, int MaxClusterSize,
	                                      vector<pcl::PointIndices> * out_indice);

//ŷʽ����
HEAD void CallingConvention euclideanCluster(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                     double distance_thresh,int MinClusterSize, int MaxClusterSize,
	                                     vector<pcl::PointIndices> * out_indice);


/*
���ܣ�ʹ��Ransac�㷨��Ϸָ����Ƶ�ƽ�棬������ƽ����б�Ƕ�
param[in] in_pc Ŀ����ƶ���ָ��
param[in] distance_thresh Ransac�㷨������ֵ
param[in] max_itera Ransac�㷨����������
param[out] normal ���ƽ��ķ���ϵ�������ΰ���a��b��c��d�ĸ�ֵ��������ʽΪax+by+cz+d=0
*/
HEAD float CallingConvention fitPlane(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                  float distance_thresh, int max_itera, float * normal);


/*
���ܣ�����б�ĵ���ƽ��У����Ҳ���ǽ�����ƽ��任����ˮƽ��ƽ��
param[in] in_x Ŀ����ƶ���ָ��
param[in] normal ������Ƶķ�����
param[out] out_x ������ƶ����ָ��
*/
HEAD void CallingConvention correctPlane(pcl::PointCloud<pcl::PointXYZ> * in_pc, float * normal,
	                                     pcl::PointCloud<pcl::PointXYZ> * out_pc);


/*
���ܣ�sigam�����޳��쳣ֵ
param[in] in_pc Ŀ����ƶ���ָ��
param[in] sigam_thresh   sigam������ֵ��һ��Ϊ3
param[out] out_x ������ƶ���ָ��
*/
HEAD void CallingConvention sigamFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc, int sigam_thresh,
	                                    pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ������ư����з���洢��ÿһ�д���һ������Բ�ܣ�������û�ָ����n�в���Բ�ܵ���
param[in] in_pc Ŀ����ƶ���ָ��
param[in] num   ָ����������в���Բ�ܵ��ƣ�һ����˵��nԽ��Խ�ƽ�����ȫ����
��nΪ�������Ƶ�����Ŀ������ľ����������ƣ������Ľ��ʵ�����Ƕ���ȫ������nһ��Ĭ��Ϊ20
param[out] out_x ������ƶ���ָ��
*/
HEAD void CallingConvention getRunoutPoints(pcl::PointCloud<pcl::PointXYZ> * in_pc, int num,
	                                        pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ������Լ����ԣ�ֱ�ӷ��ض���������������ṩ�ӿ�
�����ư����з���洢��ÿһ�д���һ������Բ�ܣ�������û�ָ����n�в���Բ�ܵ���
//���һ����������귨���޳��쳣ֵ�����������յĶ�������
param[in] in_x Ŀ�����x����
param[in] in_y Ŀ�����y����
param[in] in_z Ŀ�����z����
param[in] num   ָ����������в���Բ�ܵ��ƣ�һ����˵��nԽ��Խ�ƽ�����ȫ����
��nΪ�������Ƶ�����Ŀ������ľ����������ƣ������Ľ��ʵ�����Ƕ���ȫ������nһ��Ĭ��Ϊ20
param[out] out_x ������Ƶ�X����
param[out] out_y ������Ƶ�Y����
param[out] out_z ������Ƶ�Z����
*/
HEAD float CallingConvention getRunoutPointsWithResult(pcl::PointCloud<pcl::PointXYZ> * in_pc, int num,
	                                                   pcl::PointCloud<pcl::PointXYZ> * out_pc);

/*
���ܣ� �����������,�����ظõ�������С���������
param[in] in_pc Ŀ����ƶ���ָ��
*/
HEAD double CallingConvention calculateRunout(pcl::PointCloud<pcl::PointXYZ> * in_pc, int * indices);

/*
���ܣ� ���ݵ�����������һ�����ƶ����п���������Ӧ�ĵ�
*/
HEAD void CallingConvention copyPcBaseOnIndice(pcl::PointCloud<pcl::PointXYZ> * in_pc, 
	                                           pcl::PointIndices * in_indice,
	                                           pcl::PointCloud<pcl::PointXYZ> * out_pc);



