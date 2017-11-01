#include<vector>
#include <stdlib.h>


#define MY_BACKGROUND 0
#define MY_SCAN_LEN 4

//////////////////////////////////////////边缘标记/////////////////////////////////////////
int USTC_Find_Contours(Mat binaryImg, vector < vector < cv::Point >>& contours){
	/*contours[0][0].x; contours.begin[0][0];*/

	int mRows = binaryImg.rows;  //灰度图行数
	int mCols = binaryImg.cols;  //灰度图列数
	int bin_th = 128;
	int label_max = 0;

	Mat labelImg(binaryImg.rows, binaryImg.cols, CV_32FC1);
	int *data_label = (int *)labelImg.data;

	ustc_ConnectedComponentLabeling(binaryImg, labelImg, bin_th, label_max);


	//将标签图扩展一圈，扩充的像素值置0，并填入原图像素，这样做是为了便于处理边界点
	Mat ex_labelImg(binaryImg.rows + 2, binaryImg.cols + 2, CV_32FC1);
	ex_labelImg.setTo(MY_BACKGROUND);
	int *data_ex = (int *)ex_labelImg.data;
	int mRows_ex = ex_labelImg.rows;  //扩展标签图行数
	int mCols_ex = ex_labelImg.cols;  //扩展标签图列数
	for (int i = 0; i < binaryImg.rows; i++)
	{
		for (int j = 0; j < binaryImg.cols; j++)
		{
			data_ex[(i + 1)*mCols_ex + (j + 1)] = data_label[i*binaryImg.cols + j];
		}
	}

	///当前像素的周围像素标号
/*
	3  2  1
	4  p  0
	5  6  7
*/

	int* label_table = new int[label_max + 1]();
	int label_now=0;
	int vector_num = 0;
		for (int i = 1; i < mRows_ex - 1; i++){
			for (int j = 1; j < mCols_ex - 1; j++){
				label_now = data_ex[(i)*mCols_ex + (j)];
				if (label_now && (!label_table[label_now])){
					label_table[label_now] = 1;                       //被标记过
					Point pt_start(i-1, j-1);
					vector_num++;
					contours[vector_num].push_back(pt_start);
					int t_end = 0;                        //终结点个数，对于非封闭曲线
					int position_now = 6;//0;
					int x=i, y=j;
					//轮廓搜索
					do{
						switch (position_now){
						case 0:
							if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x+1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else{
								t_end++;
							/*	x = i;
								y = j;*/
								position_now = 4;
							}
							break;
						case 1:
							if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y ;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 5;
							}
							break;
						case 2:
							if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x ;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 6;
							}
							break;
						case 3:
							if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 7;
							}
							break;
						case 4:
							if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 0;
							}
							break;
						case 5:
							if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 1;
							}
							break;
						case 6:
							if (data_ex[(x - 1)*mCols_ex + (y - 1)] == label_now){
								x = x - 1;
								y = y - 1;
								position_now = 3;
							}
							else if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 2;
							}
							break;
						case 7:
							if (data_ex[(x)*mCols_ex + (y - 1)] == label_now){
								x = x;
								y = y - 1;
								position_now = 4;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y - 1)] == label_now){
								x = x + 1;
								y = y - 1;
								position_now = 5;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y)] == label_now){
								x = x + 1;
								y = y;
								position_now = 6;
							}
							else if (data_ex[(x + 1)*mCols_ex + (y + 1)] == label_now){
								x = x + 1;
								y = y + 1;
								position_now = 7;
							}
							else if (data_ex[(x)*mCols_ex + (y + 1)] == label_now){
								x = x;
								y = y + 1;
								position_now = 0;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y + 1)] == label_now){
								x = x - 1;
								y = y + 1;
								position_now = 1;
							}
							else if (data_ex[(x - 1)*mCols_ex + (y)] == label_now){
								x = x - 1;
								y = y;
								position_now = 2;
							}
							else{
								t_end++;
								/*x = i;
								y = j;*/
								position_now = 3;
							}
							break;
						default:
							break;
						}
						//cout << label_now << ":" << x << "," << y << " ";
						Point pt_now(x-1, y-1);
						contours[vector_num].push_back(pt_now);
						if ((pt_now == pt_start &&t_end!=1) || t_end==2){
							break;
						}
					} while (1);


				}





			}
		}
	
	


	
#ifdef SHOW
	Mat printImg(mRows_ex, mCols_ex, CV_8UC1);
	uchar *data_print = (uchar *)printImg.data;
	for (int i = 0; i < mRows_ex; i++)
	{
	for (int j = 0; j < mCols_ex; j++)
	{
		data_print[i*mCols_ex + j] = 50 *data_ex[i*mCols_ex + j];
		//cout << data_ex[i*mCols_ex + j] << " ";

	}
	cout << endl;
	}

	Mat border_labelImg(mRows_ex, mCols_ex, CV_8UC1);
	border_labelImg.setTo(MY_BACKGROUND);
	for (int i = 1; i < 1000; i++)
	{
		for (int j = 0; j < contours[i].size(); j++){
			border_labelImg.data[contours[i][j].x * mCols_ex + contours[i][j].y] = 255;
		}
	}
	//int num_print = 2;
	//for (int i = 0; i < contours[num_print].size(); i++){
	//	cout << (contours[num_print][i]).x << "," << (contours[num_print][i]).y << " ";
	//}
	
	//cout << (contours[2][0]).x << endl;

	namedWindow("Img", 0);
	imshow("Img", binaryImg);

	namedWindow("exImg", 0);
	imshow("exImg", printImg);

	namedWindow("borderImg", 0);
	imshow("borderImg", border_labelImg);
#endif
	return 0;
}




//功能说明：更新标签的连接关系
//label：“标签连接关系”的数组
//first：被连接的第一个标签
//second：被连接的第二个标签
void my_connect(int *label, int first, int second);

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat & labelImg, int bin_th,int &label_max)
{
	uchar *data_img = (uchar *)grayImg.data;
	if (NULL == data_img)
	{
		cout << "failed to read img!" << endl;
		return MY_FAIL;
	}

	int *data_label = (int *)labelImg.data;
	if (NULL == data_label)
	{
		cout << "labelImg is NULL!" << endl;
		return MY_FAIL;
	}

	int mRows = grayImg.rows;  //灰度图行数
	int mCols = grayImg.cols;  //灰度图列数
	int len = mRows*mCols / 4;  //“标签连接关系”数组的长度
	int *label = new int[len]();  //“标签连接关系”数组
	int now_label = 1;  //当前标签
	int scan[MY_SCAN_LEN];  //扫描器大小
	int num = 0;  //当前扫描器内标签个数

	//初始化“标签连接关系”数组，对应位置设成对应值
	for (int i = 0; i < len; i++)
	{
		label[i] = i;
	}

	//将标签图扩展一圈，扩充的像素值置0，并填入原图像素，这样做是为了便于处理边界点
	Mat ex_labelImg(mRows + 2, mCols + 2, CV_32FC1);
	ex_labelImg.setTo(MY_BACKGROUND);
	int *data_ex = (int *)ex_labelImg.data;
	int mRows_ex = ex_labelImg.rows;  //扩展标签图行数
	int mCols_ex = ex_labelImg.cols;  //扩展标签图列数
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			data_ex[(i + 1)*mCols_ex + (j + 1)] = data_img[i*mCols + j];
		}
	}

	//扫描扩展后的标签图
	for (int i = 1; i < mRows + 1; i++)
	{
		for (int j = 1; j < mCols + 1; j++)
		{
			int index = i*mCols_ex + j;
			int pix = data_ex[index];

			//如果是背景，不作处理，标签记为0
			if (pix < bin_th)
			{
				data_ex[index] = MY_BACKGROUND;
			}
			//如果是前景，检查其邻域（扫描器）
			else
			{
				int index_left_top = (i - 1)*mCols_ex + (j - 1);
				int label_left_top = data_ex[index_left_top];
				int label_top = data_ex[index_left_top + 1];
				int label_right_top = data_ex[index_left_top + 2];
				int label_left = data_ex[index_left_top + mCols_ex];

				//记录下扫描器内的标签值
				if (label_left_top != MY_BACKGROUND)
				{
					scan[num] = label_left_top;
					num++;
				}
				if (label_top != MY_BACKGROUND)
				{
					scan[num] = label_top;
					num++;
				}
				if (label_right_top != MY_BACKGROUND)
				{
					scan[num] = label_right_top;
					num++;
				}
				if (label_left != MY_BACKGROUND)
				{
					scan[num] = label_left;
					num++;
				}

				//扫描器内都是背景
				if (num == 0)
				{
					data_ex[index] = now_label;
					now_label++;
				}
				//扫描器内有标签
				else
				{
					//给当前位置填入扫描器内的最小标签
					int min_label = scan[0];
					for (int k = 1; k < num; k++)
					{
						if (min_label > scan[k])
						{
							min_label = scan[k];
						}
					}
					data_ex[index] = min_label;

					//更新标签之间的联通关系
					for (int k = 1; k < num; k++)
					{
						my_connect(label, scan[k - 1], scan[k]);
					}
				}
				num = 0;  //扫描器内标签数清零，为下一次扫描做准备

			}
		}
	}

	//第一次扫描完标签图后，第二次更新“标签连接关系”数组
	for (int i = 1; i < now_label; i++)
	{
		int tag = label[i];
		//找到该标签连通的最小标签值
		while (label[tag] != tag)
		{
			tag = label[tag];
		}
		label[i] = tag;
	}

	//第二次扫描标签图，完成连通域标记
	for (int i = 1; i < mRows + 1; i++)
	{
		for (int j = 1; j < mCols + 1; j++)
		{
			int index_ex = i*mCols_ex + j;
			int index = (i - 1)*mCols + (j - 1);
			int tag = data_ex[index_ex];
			data_label[index] = label[tag];
		}
	}
	label_max = now_label - 1;
	delete[]label;
	return MY_OK;
}

void my_connect(int * label, int first, int second)
{
	if (first == second)
	{
		return;
	}

	//一个记录大标签，一个记录小标签
	int label_b = first;
	int label_s = second;
	int cash;  //用于交换数据
	if (first < second)
	{
		label_b = second;
		label_s = first;
	}

	//大标签已经记录了连接关系，则转化为记录的标签与另一标签的连接关系
	while (label[label_b] != label_b)
	{
		label_b = label[label_b];
		//记录的标签与另一标签相同，说明该连接关系已被记录，直接返回
		if (label_b == label_s)
		{
			return;
		}

		//记录的标签与另一标签不同，则重复以上步骤（要确保是在大标签下记录小标签）
		if (label_b < label_s)
		{
			cash = label_b;
			label_b = label_s;
			label_s = cash;
		}
	}

	//记录标签连接关系
	label[label_b] = label_s;
	return;
}
