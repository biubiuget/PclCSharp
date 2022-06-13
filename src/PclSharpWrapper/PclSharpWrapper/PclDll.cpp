#include "PclDll.h"


void Pc2Array(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, double * out_x, double * out_y, double * out_z)
{
	for (size_t i = 0; i < cloud_in->points.size(); i++) {

		out_x[i] = cloud_in->points[i].x;
		out_y[i] = cloud_in->points[i].y;
		out_z[i] = cloud_in->points[i].z;

	}
}
//�����������������������ĵ�ص���������
int getMaxPointCluster(vector<pcl::PointIndices> cluster)
{
	int i = 0;
	int pos = 0;
	int max = 0;
	//�����������������е�Ԫ�أ����籾���еĵ�����it�ʹ���һ��pcl::pointindices
	for (vector<pcl::PointIndices>::iterator it = cluster.begin(); it != cluster.end(); ++it)
	{
		if (it->indices.size() > max)
		{
			pos = i;
			max = it->indices.size();
		}
		i = i + 1;
	}
	return pos;
}
//����ƽ������л����Ϻ��ƽ����xy�ο������б��
double getAngle(double line1[3], double line2[3])
{
	double cos = abs(line1[0] * line2[0] + line1[1] * line2[1] + line1[2] * line2[2]) /
		sqrt(line1[0] * line1[0] + line1[1] * line1[1] + line1[2] * line1[2]) * sqrt(line2[0] * line2[0] + line2[1] * line2[1] + line2[2] * line2[2]);
	double deg = std::acos(cos);

	return deg * 180 / 3.1415926;
}
//���ڻ�ȡ�������ı���ת������
Eigen::Matrix4f getRotationMatrix(Eigen::Vector3f vector_before, Eigen::Vector3f vector_after)
{
	vector_before.normalize();
	vector_after.normalize();
	float angle = acos(vector_before.dot(vector_after));//������õ��������ļн�
	Eigen::Vector3f p_rotate = vector_before.cross(vector_after);//������õ��Ļ�������
	p_rotate.normalize();//�������ĵ�λ����������ת��ĵ�λ����
	Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
	rotationMatrix(0, 0) = cos(angle) + p_rotate[0] * p_rotate[0] * (1 - cos(angle));
	rotationMatrix(0, 1) = p_rotate[0] * p_rotate[1] * (1 - cos(angle) - p_rotate[2] * sin(angle));//�������ʽ�ȶ���һ�����ţ����ǿ�ʵ�������ǶԵġ�
	rotationMatrix(0, 2) = p_rotate[1] * sin(angle) + p_rotate[0] * p_rotate[2] * (1 - cos(angle));

	rotationMatrix(1, 0) = p_rotate[2] * sin(angle) + p_rotate[0] * p_rotate[1] * (1 - cos(angle));
	rotationMatrix(1, 1) = cos(angle) + p_rotate[1] * p_rotate[1] * (1 - cos(angle));
	rotationMatrix(1, 2) = -p_rotate[0] * sin(angle) + p_rotate[1] * p_rotate[2] * (1 - cos(angle));

	rotationMatrix(2, 0) = -p_rotate[1] * sin(angle) + p_rotate[0] * p_rotate[2] * (1 - cos(angle));
	rotationMatrix(2, 1) = p_rotate[0] * sin(angle) + p_rotate[1] * p_rotate[2] * (1 - cos(angle));
	rotationMatrix(2, 2) = cos(angle) + p_rotate[2] * p_rotate[2] * (1 - cos(angle));
	return rotationMatrix;
}
//��������������Ϊ���ƶ���
void connect_pc(vector<pcl::PointCloud<pcl::PointXYZ>>& input_pc1, pcl::PointCloud<pcl::PointXYZ> & out_cloud)
{
	for (int i = 0; i < input_pc1.size(); i++)
	{
		out_cloud = out_cloud + input_pc1[i];
	}
}

//�������أ�3�����귨���޳��쳣ֵ�������ǵ��ƣ�����ǵ�������
void stdDev_filter(vector<pcl::PointCloud<pcl::PointXYZ>>& input_cloud, vector<pcl::PointCloud<pcl::PointXYZ>>& out_cloud)
{
	double mean = 0;
	double stdDev = 0;
	vector<float> vec_z;
	out_cloud.resize(input_cloud.size());
	//������Ƶľ�ֵ�ͱ�׼��
	for (int i = 0; i < input_cloud.size(); i++)
	{
		for (int k = 0; k < input_cloud[i].points.size(); k++)
		{
			vec_z.push_back(input_cloud[i].points[k].z);
		}

	}
	pcl::getMeanStdDev(vec_z, mean, stdDev);
	//��2������׼���޳���
	for (int j = 0; j < input_cloud.size(); j++)
	{
		for (int d = 0; d < input_cloud[j].points.size(); d++)
		{
			//�п��ܻ����ĳ���ߵĵ�ȫ�����޳����������ʱ��õ���û�е�᲻�ᱨ��
			if (fabs(input_cloud[j].points[d].z - mean) < 2 * stdDev)
			{
				out_cloud[j].push_back(input_cloud[j].points[d]);
			}
		}

	}
}

HEAD int CallingConvention loadFile(char * path, double * out_x, double * out_y, double * out_z)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
	if (pcl::io::loadPLYFile(path, *cloud) == -1)
	{
		return 0;
	}
	else
	{
		Pc2Array(cloud, out_x, out_y, out_z);
		return 1;
	}
}
//����ply�ļ�
HEAD int CallingConvention loadPlyFile(char * path, pcl::PointCloud<pcl::PointXYZ>* pc)
{
	if (pcl::io::loadPLYFile(path, *pc) == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
//����pcd�ļ�
HEAD int CallingConvention loadPcdFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc)
{
	if (pcl::io::loadPCDFile(path, *pc) == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}
//����pcd�ļ�
HEAD void CallingConvention savePcdFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc, int binaryMode)
{
	if (binaryMode >= 1)
	{
		pcl::io::savePCDFile(path, *pc, true);
	}
	else
	{
		pcl::io::savePCDFile(path, *pc, false);
	}

}
//����ply�ļ�
HEAD void CallingConvention savePlyFile(char* path, pcl::PointCloud<pcl::PointXYZ> * pc, int binaryMode)
{
	if (binaryMode >= 1)
	{
		pcl::io::savePLYFile(path, *pc, true);
	}
	else
	{
		pcl::io::savePLYFile(path, *pc, false);
	}

}


//�����²���
HEAD void CallingConvention uniformDownSample(pcl::PointCloud<pcl::PointXYZ> * in_pc, double radius,
	pcl::PointCloud<pcl::PointXYZ> * out_pc)
{

	pcl::UniformSampling<pcl::PointXYZ> uniform;
	uniform.setInputCloud(in_pc->makeShared());
	uniform.setRadiusSearch(radius);
	uniform.filter(*out_pc);

}
//ֱͨ�˲�
HEAD void CallingConvention passThroughFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc, char * axis_name,
	float limit_min, float limit_max, int negative,
	pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(in_pc->makeShared());
	if (negative <= 0)
	{
		pass.setNegative(false);
		pass.setFilterFieldName(axis_name);
		pass.setFilterLimits(limit_min, limit_max);
		pass.filter(*out_pc);
	}
	else
	{
		pass.setNegative(true);
		pass.setFilterFieldName(axis_name);
		pass.setFilterLimits(limit_min, limit_max);
		pass.filter(*out_pc);
	}
}
//ͳ���˲�
HEAD void CallingConvention staFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	int neighbor_num, float thresh,
	pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sta;
	sta.setInputCloud(in_pc->makeShared());
	sta.setMeanK(neighbor_num);
	sta.setStddevMulThresh(thresh);
	sta.filter(*out_pc);

}

/*
���ܣ��뾶�˲�������Ⱥ��ȥ��
*/
HEAD void CallingConvention radiusFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                     double radius, int num_thresh,
	                                     pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	pcl::RadiusOutlierRemoval<pcl::PointXYZ> ror;
	ror.setInputCloud(in_pc->makeShared());
	ror.setRadiusSearch(radius);
	ror.setMinNeighborsInRadius(num_thresh);
	ror.filter(*out_pc);
}

//ԭʼ����������
HEAD void CallingConvention oriGrowRegion(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                      int neighbor_num, float smooth_thresh, float curva_thresh,
	                                      int MinClusterSize, int MaxClusterSize,
	                                      vector<pcl::PointIndices> * out_indice)
{
	//������Ƶķ�����
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>());
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(in_pc->makeShared());
	ne.setSearchMethod(tree);
	ne.setKSearch(neighbor_num);
	ne.compute(*normals);

	//��������
	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> rg;
	rg.setSearchMethod(tree);
	rg.setInputCloud(in_pc->makeShared());
	rg.setInputNormals(normals);
	rg.setMinClusterSize(MinClusterSize);
	rg.setMaxClusterSize(MaxClusterSize);
	rg.setCurvatureThreshold(curva_thresh);
	rg.setNumberOfNeighbours(neighbor_num);
	rg.setSmoothnessThreshold(smooth_thresh / 180.0 * M_PI);//Ҫ���ǻ��ȣ�������ǽǶȣ�����ת��һ��

	//��ȡ�����ĵ��
	//vector<pcl::PointIndices> cluster;
	rg.extract(*out_indice);

}

//ŷʽ����
HEAD void CallingConvention euclideanCluster(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                      double distance_thresh, int MinClusterSize, int MaxClusterSize,
	                                      vector<pcl::PointIndices> * out_indice)
{
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	tree->setInputCloud(in_pc->makeShared());

	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
	ec.setClusterTolerance(distance_thresh);
	ec.setMinClusterSize(MinClusterSize);
	ec.setMaxClusterSize(MaxClusterSize);
	ec.setSearchMethod(tree);
	ec.setInputCloud(in_pc->makeShared());
	ec.extract(*out_indice);

}

//��װ�������������ֱ�ӷ��ص���������ƽ��
HEAD void CallingConvention modifiedGrowRegion(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	int neighbor_num, float smooth_thresh, float curva_thresh,
	int MinClusterSize, int MaxClusterSize,
	pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	//������Ƶķ�����
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>());
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(in_pc->makeShared());
	ne.setSearchMethod(tree);
	ne.setKSearch(neighbor_num);
	ne.compute(*normals);

	//��������
	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> rg;
	rg.setSearchMethod(tree);
	rg.setInputCloud(in_pc->makeShared());
	rg.setInputNormals(normals);
	rg.setMinClusterSize(MinClusterSize);
	rg.setMaxClusterSize(MaxClusterSize);
	rg.setCurvatureThreshold(curva_thresh);
	rg.setNumberOfNeighbours(neighbor_num);
	rg.setSmoothnessThreshold(smooth_thresh / 180.0 * M_PI);//Ҫ���ǻ��ȣ�������ǽǶȣ�����ת��һ��

	//��ȡ�����ĵ��
	vector<pcl::PointIndices> cluster;
	rg.extract(cluster);
	
	//�ҵ���������ƽ��
	int pos = getMaxPointCluster(cluster);
	pcl::copyPointCloud(*in_pc, cluster[pos], *out_pc);


}
//���ƽ��
HEAD float CallingConvention fitPlane(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	float distance_thresh, int max_itera, float * normal)
{
	float angel = 0.0;
	pcl::ModelCoefficients::Ptr modelCoeff(new pcl::ModelCoefficients());
	pcl::PointIndices::Ptr pointIndices(new pcl::PointIndices());
	pcl::SACSegmentation<pcl::PointXYZ> seg;
	seg.setModelType(pcl::SACMODEL_PLANE);
	seg.setMethodType(pcl::SAC_RANSAC);
	seg.setInputCloud(in_pc->makeShared());
	seg.setDistanceThreshold(distance_thresh);
	seg.setMaxIterations(max_itera);
	seg.setOptimizeCoefficients(true);
	seg.segment(*pointIndices, *modelCoeff);

	double line1[3] = { 0,0,1 };//�ο�ƽ������
	double line2[3] = { modelCoeff->values[0],modelCoeff->values[1],modelCoeff->values[2] };//���ƽ������
	angel = getAngle(line1, line2);//��ȡ��������ļн�
	normal[0] = modelCoeff->values[0];
	normal[1] = modelCoeff->values[1];
	normal[2] = modelCoeff->values[2];
	normal[3] = modelCoeff->values[3];
	return angel;

}

//У��ƽ��
HEAD void CallingConvention correctPlane(pcl::PointCloud<pcl::PointXYZ> * in_pc, float * normal,
	pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	Eigen::Vector3f vector;
	vector << normal[0], normal[1], normal[2];
	Eigen::Vector3f vector_after(0, 0, 1);
	//�����б�任�ľ���
	Eigen::Matrix4f rotion_matrix = getRotationMatrix(vector, vector_after);
	//����б�ĵ���ƽ�����任��У���ط�����Ϊ��0,0,1����ƽ��
	pcl::transformPointCloud(*in_pc, *out_pc, rotion_matrix);

}

//sigam�����޳���ֵ
HEAD void CallingConvention sigamFilter(pcl::PointCloud<pcl::PointXYZ> * in_pc, int sigam_thresh,
	                                    pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	double mean = 0;
	double stdDev = 0;
	vector<float> vec_z;
	for (int i = 0; i < in_pc->points.size(); i++)
	{
		vec_z.push_back(in_pc->points[i].z);
	}
	pcl::getMeanStdDev(vec_z, mean, stdDev);
	for (int j = 0; j < in_pc->points.size(); j++)
	{
		if (fabs(in_pc->points[j].z - mean) < sigam_thresh * stdDev)
		{
			out_pc->push_back(in_pc->points[j]);
		}
	}

}

//���ܣ������ư����з���洢��ÿһ�д���һ������Բ�ܣ�������û�ָ����n�в���Բ�ܵ���
HEAD void CallingConvention getRunoutPoints(pcl::PointCloud<pcl::PointXYZ> * in_pc, int num,
	                                        pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	/*��ԭʼ���ư����еķ�ʽ�洢�����������е�ÿһ��Ԫ�ض���һ�����ƶ���һ�����ƶ���洢����һ�������ߵĵ���
	  */
	vector<pcl::PointCloud<pcl::PointXYZ>> out_res;//�����ư��д洢
	vector<pcl::PointCloud<pcl::PointXYZ>> out_std;//�޳���Ч�е���֮�������֯����
	vector<pcl::PointCloud<pcl::PointXYZ>> out_pc1;//�����ư��д洢
	vector<pcl::PointCloud<pcl::PointXYZ>> out_pc2;//�޳��쳣ֵ��İ��д洢����

	int y_interval = 100;//ÿһ�������ߵ�y�����100,�������������Ĳ��������Ե���
	int y_pc = 0;
	pcl::PointXYZ minPt(0, 0, 0), maxPt(0, 0, 0);
	pcl::getMinMax3D(*in_pc, minPt, maxPt);//�ҵ�y�ļ�ֵ
	int y_min = round(minPt.y);
	int y_max = round(maxPt.y);
	//����100����Ϊÿһ�������ߵ�y�����100��
	//y�������Сֵ֮���ٳ��Լ���õ����Ǽ����������������и����⣬����ָ��ʱ���е�����
	//���²�����ÿ��һ��һ����һ�������ߣ�Ҳ�п��ܸ��˺ü��ٲ���һ�������ߡ���������Ľ���ͻ�ƫ��
	//��Ŀǰ���Ե�ʱ��û�����������������ʱ�ǰ�ȫ�ġ�2022.5.24
	int line_nums = (y_max - y_min) / y_interval;
	line_nums = line_nums + 1;
	int pc_num = in_pc->points.size();//��������
	//ǰ��Ĺ�������Ϊ��ȷ���ж����������ߣ��ó�ʼ��vector
	//resize֮��ע���ڴ�˳������,��һ��û�н�����
	out_res.resize(line_nums);
	//��ʼ�����ư����д洢
	int k = 0;
	int flag = 0;
	for (int i = y_min; i <= y_max; i = i + 100)//�ܹ��ж�����������
	{
		for (int j = 0; j < pc_num; j++)//�����е㼯����yֵһ���ĵ㣬yֵһ���ĵ㱻��Ϊ��һ�������ߵĵ���
		{
			y_pc = round(in_pc->points[j].y);
			if (y_pc == i)
			{

				out_res[k].push_back(in_pc->points[j]);
				flag = flag + 1;//ͳ��һ�������ж��ٸ���
			}
		}
		//�����K����û�е㣬����һ�������䣬��ֹ����
		//������Ԥ����һ������Ŀƫ������ЩԪ��Ϊ�գ��������ᵽ�����һ�������ʽ.���˸о����Ǻܺã��������ܲ��ܸĽ�
		if (flag == 0)
		{
			pcl::PointXYZ p(0, 0, 0);
			out_res[k].push_back(p);
		}
		k++;
		flag = 0;
	}
	//�޳�һЩ���������ߡ����ƾ����д洢֮�󣬿϶�����һЩ�����ߵ������٣���Щ�������ٵĵ���Ӧ�ñ��޳�
	for (int i = 0; i < out_res.size(); i++)
	{
		//С��70�����������Ϊ����Ч�����ߣ�ʵ���Ϸ�ӳ�������ı�Եë�̣�ֱ������
		if (70 <= out_res[i].points.size()) //70��240������Ϊ����Ч�����ߣ����Ǹü����߰������ɿ׶�
		{
			out_std.push_back(out_res[i]);
		}

	}

	//Ŀ�ģ��������ռ�����Ƕ����������д洢���ƻ����ܽ�����⡣���滹��Ҫ���д洢���Ʊ�Ϊ�д洢���ƣ�
	//��Ϊÿһ�е��ƲŴ���һ������Բ�ܡ�

	/*�������ĵ��Ʋ�������ѵģ���Щ���Ƶ�����࣬���м���ܿ���һ�飬������ĵ�����һ�鲢û�п�
	Ӧ�ý�����xֵ����һ�������У����ڴˣ�ʹ�ü���set��set��������Ҫ���ԣ�һ����
	û���ظ�Ԫ�أ���һ���ǻ��Զ�������������*/

	//��õ������ж�����
	set<int, greater<int>> x_set;//��������
	for (int i = 0; i < out_std.size(); i++)
	{
		for (int j = 0; j < out_std[i].points.size(); j++)
		{
			//������������Ϊ���ȹ�ϵ�����ܻ���С�������Ҿ���ǰ���У��ƽ��֮�󣬵��������Ҳ����С��
			//����������Ҫʹ��round�����ݽ���Բ��
			x_set.insert(round(out_std[i].points[j].x));
		}
	}
	//Ϊ�˷����������x_set����תΪvector
	vector<int> pc_x;
	for (set<int>::iterator it = x_set.begin(); it != x_set.end(); it++)
	{
		pc_x.push_back(*it);
	}


	out_pc1.resize(pc_x.size());

	//��x��ֵΪ������xֵһ����Ϊͬһ��
	//������㷨���Ӷ�Ϊn3����Ҫ�Ľ�
	for (int i = 0; i < pc_x.size(); i++)
	{
		for (int j = 0; j < out_std.size(); j++)
		{
			for (int k = 0; k < out_std[j].points.size(); k++)
			{
				//xֵһ������Ϊ��ͬһ�е�
				if (round(pc_x[i]) == round(out_std[j].points[k].x))
				{
					out_pc1[i].push_back(out_std[j].points[k]);
					//ÿһ��ֻ��һ��һ���ģ�һ���ҵ����������ѭ������ʡʱ��
					break;
				}
			}
		}
	}
	//��ÿһ���е������ٵĵ��޳�
	for (int i = 0; i < out_pc1.size(); i++)
	{
		if (out_pc1[i].points.size() > 120)
		{
			out_pc2.push_back(out_pc1[i]);
		}
	}


	//���ѡ��n������Բ�ܣ����������
	vector<pcl::PointCloud<pcl::PointXYZ>> out_resVec;//����ָ���Ķ���Բ��
	int column_size = out_pc2.size();
	//����������Ϊ��������һ���ܱ�10���������Կ������������
	int surplus = column_size % 10;
	//���������������ʣ�µ�������10�ı���������ָ����Բ����
	//�Ϳ��Եõ�ÿһ��Բ�ܵļ����������ֻ������10
	//���ǲ���³��
	int gap = (column_size - surplus) / num;
	for (int i = surplus; i < out_pc2.size(); i = i + gap)
	{
		out_resVec.push_back(out_pc2[i]);
	}
	connect_pc(out_resVec, *out_pc);

}

//�����Լ����ԣ��������ṩ�ӿ�
HEAD float CallingConvention getRunoutPointsWithResult(pcl::PointCloud<pcl::PointXYZ> * in_pc, int num,
	                                                   pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	/*��ԭʼ���ư����еķ�ʽ�洢�����������е�ÿһ��Ԫ�ض���һ�����ƶ���һ�����ƶ���洢����һ�������ߵĵ���
	  */
	vector<pcl::PointCloud<pcl::PointXYZ>> out_res;//�����ư��д洢
	vector<pcl::PointCloud<pcl::PointXYZ>> out_std1;//�޳���Ч�е���֮�������֯����
	vector<pcl::PointCloud<pcl::PointXYZ>> out_std;//sigam�����޳��쳣ֵ�����������
	vector<pcl::PointCloud<pcl::PointXYZ>> out_pc1;//�����ư��д洢
	vector<pcl::PointCloud<pcl::PointXYZ>> out_pc2;//�޳��쳣ֵ��İ��д洢����

	int y_interval = 100;//ÿһ�������ߵ�y�����100,�������������Ĳ��������Ե���
	int y_pc = 0;
	pcl::PointXYZ minPt(0, 0, 0), maxPt(0, 0, 0);
	pcl::getMinMax3D(*in_pc, minPt, maxPt);//�ҵ�y�ļ�ֵ
	int y_min = round(minPt.y);
	int y_max = round(maxPt.y);
	//����100����Ϊÿһ�������ߵ�y�����100��
	//y�������Сֵ֮���ٳ��Լ���õ����Ǽ����������������и����⣬����ָ��ʱ���е�����
	//���²�����ÿ��һ��һ����һ�������ߣ�Ҳ�п��ܸ��˺ü��ٲ���һ�������ߡ���������Ľ���ͻ�ƫ��
	//��Ŀǰ���Ե�ʱ��û�����������������ʱ�ǰ�ȫ�ġ�2022.5.24
	int line_nums = (y_max - y_min) / y_interval;
	line_nums = line_nums + 1;
	int pc_num = in_pc->points.size();//��������
	//ǰ��Ĺ�������Ϊ��ȷ���ж����������ߣ��ó�ʼ��vector
	//resize֮��ע���ڴ�˳������,��һ��û�н�����
	out_res.resize(line_nums);
	//��ʼ�����ư����д洢
	int k = 0;
	int flag = 0;
	for (int i = y_min; i <= y_max; i = i + 100)//�ܹ��ж�����������
	{
		for (int j = 0; j < pc_num; j++)//�����е㼯����yֵһ���ĵ㣬yֵһ���ĵ㱻��Ϊ��һ�������ߵĵ���
		{
			y_pc = round(in_pc->points[j].y);
			if (y_pc == i)
			{

				out_res[k].push_back(in_pc->points[j]);
				flag = flag + 1;//ͳ��һ�������ж��ٸ���
			}
		}
		//�����K����û�е㣬����һ�������䣬��ֹ����
		//������Ԥ����һ������Ŀƫ������ЩԪ��Ϊ�գ��������ᵽ�����һ�������ʽ.���˸о����Ǻܺã��������ܲ��ܸĽ�
		if (flag == 0)
		{
			pcl::PointXYZ p(0, 0, 0);
			out_res[k].push_back(p);
		}
		k++;
		flag = 0;
	}
	//�޳�һЩ���������ߡ����ƾ����д洢֮�󣬿϶�����һЩ�����ߵ������٣���Щ�������ٵĵ���Ӧ�ñ��޳�
	for (int i = 0; i < out_res.size(); i++)
	{
		//С��70�����������Ϊ����Ч�����ߣ�ʵ���Ϸ�ӳ�������ı�Եë�̣�ֱ������
		if (70 <= out_res[i].points.size()) //70��240������Ϊ����Ч�����ߣ����Ǹü����߰������ɿ׶�
		{
			out_std1.push_back(out_res[i]);
		}

	}
	//�޳��쳣ֵ������ʵ��
	stdDev_filter(out_std1, out_std);

	//Ŀ�ģ��������ռ�����Ƕ����������д洢���ƻ����ܽ�����⡣���滹��Ҫ���д洢���Ʊ�Ϊ�д洢���ƣ�
	//��Ϊÿһ�е��ƲŴ���һ������Բ�ܡ�

	/*�������ĵ��Ʋ�������ѵģ���Щ���Ƶ�����࣬���м���ܿ���һ�飬������ĵ�����һ�鲢û�п�
	Ӧ�ý�����xֵ����һ�������У����ڴˣ�ʹ�ü���set��set��������Ҫ���ԣ�һ����
	û���ظ�Ԫ�أ���һ���ǻ��Զ�������������*/

	//��õ������ж�����
	set<int, greater<int>> x_set;//��������
	for (int i = 0; i < out_std.size(); i++)
	{
		for (int j = 0; j < out_std[i].points.size(); j++)
		{
			//������������Ϊ���ȹ�ϵ�����ܻ���С�������Ҿ���ǰ���У��ƽ��֮�󣬵��������Ҳ����С��
			//����������Ҫʹ��round�����ݽ���Բ��
			x_set.insert(round(out_std[i].points[j].x));
		}
	}
	//Ϊ�˷����������x_set����תΪvector
	vector<int> pc_x;
	for (set<int>::iterator it = x_set.begin(); it != x_set.end(); it++)
	{
		pc_x.push_back(*it);
	}


	out_pc1.resize(pc_x.size());

	//��x��ֵΪ������xֵһ����Ϊͬһ��
	//������㷨���Ӷ�Ϊn3����Ҫ�Ľ�
	for (int i = 0; i < pc_x.size(); i++)
	{
		for (int j = 0; j < out_std.size(); j++)
		{
			for (int k = 0; k < out_std[j].points.size(); k++)
			{
				//xֵһ������Ϊ��ͬһ�е�
				if (round(pc_x[i]) == round(out_std[j].points[k].x))
				{
					out_pc1[i].push_back(out_std[j].points[k]);
					//ÿһ��ֻ��һ��һ���ģ�һ���ҵ����������ѭ������ʡʱ��
					break;
				}
			}
		}
	}
	//��ÿһ���е������ٵĵ��޳�
	for (int i = 0; i < out_pc1.size(); i++)
	{
		if (out_pc1[i].points.size() > 120)
		{
			out_pc2.push_back(out_pc1[i]);
		}
	}


	//���ѡ��n������Բ�ܣ����������
	vector<pcl::PointCloud<pcl::PointXYZ>> out_resVec;//����ָ���Ķ���Բ��
	float runout = 0;
	float res = 0;
	int column_size = out_pc2.size();
	//����������Ϊ��������һ���ܱ�10���������Կ������������
	int surplus = column_size % 10;
	//���������������ʣ�µ�������10�ı���������ָ����Բ����
	//�Ϳ��Եõ�ÿһ��Բ�ܵļ����������ֻ������10
	//���ǲ���³��
	int gap = (column_size - surplus) / num;
	vector<float> runouts;
	pcl::PointXYZ min_pt(0, 0, 0);
	pcl::PointXYZ max_pt(0, 0, 0);
	for (int i = surplus; i < out_pc2.size(); i = i + gap)
	{
		out_resVec.push_back(out_pc2[i]);
		pcl::getMinMax3D(out_pc2[i], min_pt, max_pt);
		runout = max_pt.z - min_pt.z;
		runouts.push_back(runout);
	}
	connect_pc(out_resVec, *out_pc);
	//res = *max_element(runouts.begin(), runouts.end());
	for (int g = 0; g < runouts.size(); g++)
	{
		if (res <= runouts[g])
		{
			res = runouts[g];
		}
	}
	return res;
}


HEAD double CallingConvention calculateRunout(pcl::PointCloud<pcl::PointXYZ> * in_pc, int * indices)
{
	int min_indice = 0;
	int max_indice = 0;
	pcl::PointXYZ min_point(0, 0, 0);
	pcl::PointXYZ max_point(0, 0, 0);
	pcl::getMinMax3D(*in_pc, min_point, max_point);
	//�ҵ�Zֵ��С������������

	int len1 = in_pc->points.size();

	for (int i = 0; i < len1; i++)
	{
		double z = in_pc->points[i].z;
		if (z == min_point.z)
		{
			min_indice = i;
		}
		else if (z == max_point.z)
		{
			max_indice = i;
		}
	}
	indices[0] = min_indice;
	indices[1] = max_indice;
	double detect_result = max_point.z - min_point.z;//�������
	return detect_result;
}

HEAD void CallingConvention copyPcBaseOnIndice(pcl::PointCloud<pcl::PointXYZ> * in_pc,
	                                           pcl::PointIndices * in_indice,
	                                           pcl::PointCloud<pcl::PointXYZ> * out_pc)
{
	pcl::copyPointCloud(*in_pc, *in_indice, *out_pc);
}