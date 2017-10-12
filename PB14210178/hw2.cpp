#ifndef _H_SUB_IMAGE_MATCH_H_
#define _H_SUB_IMAGE_MATCH_H_

#define SUB_IMAGE_MATCH_OK 1
#define SUB_IMAGE_MATCH_FAIL -1

#define MY_OK   1
#define MY_FAIL 0

#define CIRCLE_CENTRE_SCALE  15
#define CIRCLE_CENTRE_SCALE_square  CIRCLE_CENTRE_SCALE*CIRCLE_CENTRE_SCALE
#define CIRCLE_R_SCALE  10
#define LENGTH_ENOUGH   30000//1920*1080*10


#define IMG_SHOW 
int ustc_Find_Circles_By_Difference(Mat colorImg, int min_radius, int max_radius, int max_circle_diff, int* x, int* y, int* len){

	
	int row, col;
	float angle_sin_table[360], angle_cos_table[360];
	int circle_all_r[LENGTH_ENOUGH] = { 0 };
	int diff_all[LENGTH_ENOUGH] = { 0 };
	int posi = 0;                       //数组位置下标
	int r_count_max = 10; 
	row = colorImg.rows;
	col = colorImg.cols;
	
	uchar *color_data = colorImg.data;

	Mat red_disImg(colorImg.size(), CV_32SC1);
	red_disImg.setTo(0);
	int *red_disdata = ((int*)red_disImg.data);

	Mat blue_disImg(colorImg.size(), CV_32SC1);
	blue_disImg.setTo(0);
	int *blue_disdata = ((int*)blue_disImg.data);

	Mat green_disImg(colorImg.size(), CV_32SC1);
	green_disImg.setTo(0);
	int *green_disdata = ((int*)green_disImg.data);


	if (NULL == colorImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}


	if (5 >= min_radius)
	{
		cout << "min_radius too little" << endl;
		return MY_FAIL;
	}

	//if (col <= max_radius + 5 || row <= max_radius + 5)
	//{
	//	cout << "max_radius too huge" << endl;
	//	return MY_FAIL;
	//}


	for (int angle = 0; angle < 360; angle++){
		float angle_real = angle*CV_PI / 180;
		angle_sin_table[angle] = sin(angle_real);
		angle_cos_table[angle] = cos(angle_real);
	}


	float max = 0;
	int max_i = 0, max_j = 0;

	int aver_front_five[10] = { 0 };

	//min_radius = min_radius - 5;
	//max_radius = max_radius - 5;
	for (int circle_r = min_radius; circle_r <= max_radius; circle_r++){

        int dis_outline_row =  0  ,dis_inline_row = 0;
		int dis_outline_col =  0  ,dis_inline_col = 0;
        //数组清零
		red_disImg.setTo(0); 
		blue_disImg.setTo(0);
		green_disImg.setTo(0);

		int circle_min_plus = circle_r + 5;

		if (circle_min_plus << 1 > row || circle_min_plus << 1 > col){
			break;
		}

		for (int angle = 0; angle < 360; angle++){

			dis_outline_row = (circle_r + 5)*angle_sin_table[angle];
			dis_outline_col = (circle_r + 5)*angle_cos_table[angle];

			dis_inline_row = (circle_r - 5)*angle_sin_table[angle];
			dis_inline_col = (circle_r - 5)*angle_cos_table[angle];

			for (int i = circle_min_plus; i < row - circle_min_plus; i++){

				uchar *out_row_start = &colorImg.data[3 * ((i + dis_outline_row) *col)];
				uchar *in_row_start = &colorImg.data[3 * ((i + dis_inline_row) *col)];

				for (int j = circle_min_plus; j < col - circle_min_plus; j++){

					//int outline_row =  i + dis_outline_row ;
					//int outline_col =  j + dis_inline_col ;

					//if (outline_row >= 0 && outline_row < row && outline_col >= 0 && outline_col < col){
						int dis_place = i*col + j;
						//int out_place = ((i + dis_outline_row) *col + j + dis_outline_col);
						//int in_place = ((i + dis_inline_row) *col + j + dis_inline_col);
						//uchar *out_start = &colorImg.data[3 * ((i + dis_outline_row) *col + j + dis_outline_col) ];
						//uchar *in_start = &colorImg.data[3 * ((i + dis_inline_row) *col + j + dis_inline_col) ];

						uchar *out_start = out_row_start + 3*(j + dis_outline_col) ;
						uchar *in_start = in_row_start + 3*(j + dis_inline_col);

						//red_disdata[dis_place] += colorImg.data[3 * out_place + 2] - colorImg.data[3 * in_place + 2];
						//green_disdata[dis_place] += colorImg.data[3 * out_place + 1] - colorImg.data[3 * in_place + 1];
						//blue_disdata[dis_place] += colorImg.data[3 * out_place + 0] - colorImg.data[3 * in_place + 0];

						red_disdata[dis_place] += out_start[2] - in_start[2];
						green_disdata[dis_place] += out_start[1] - in_start[1];
						blue_disdata[dis_place] += out_start[0] - in_start[0];
					//}
					

				}
			}
		}

		//统计超过阈值的差
		for (int i = 0; i < row; i++){
			for (int j = 0; j < col; j++){
				
				int red_dis_get = red_disdata[i*col + j] / 360;
				int blue_dis_get = blue_disdata[i*col + j] / 360;
				int green_dis_get = green_disdata[i*col + j] / 360;

				int place_cen = { 0 };

				if (red_dis_get > max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = red_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << red_dis_get << endl;
				}
				else if (red_dis_get < -max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = -red_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << red_dis_get << endl;
				}
				else if (blue_dis_get>max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = blue_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << blue_dis_get << endl;
				}
				else if (blue_dis_get < -max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = -blue_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << blue_dis_get << endl;
				}
				else if (green_dis_get>max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = green_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << green_dis_get << endl;
				}
				else if (green_dis_get<-max_circle_diff){
					x[posi] = i;
					y[posi] = j;
					circle_all_r[posi] = circle_r;
					diff_all[posi] = -green_disdata[i*col + j];
					place_cen = 1;
					posi++;

					//cout << green_dis_get << endl;
				}
				

#ifdef IMG_SHOW
				if (place_cen){
					colorImg.data[3 * (i*col + j) + 1] = 255 - colorImg.data[3 * (i*col + j) + 1];
					colorImg.data[3 * (i*col + j) + 2] = 255 - colorImg.data[3 * (i*col + j) + 2];
					colorImg.data[3 * (i*col + j)] = 255 - colorImg.data[3 * (i*col + j)];                  //标记圆心
				}
#endif

			}
		}

		

	}

	cout << "posi:" <<posi<< endl;




	uchar circle_sign[LENGTH_ENOUGH] = { 0 };

	//冒泡排序，按照差分值
	for (int i = 0; i<posi/*diff_all[i] != 0 && i < LENGTH_ENOUGH*/; i++){

		int max_i = i;
		int diff_max = diff_all[i];
		for (int j = i + 1; j<posi/*diff_all[j] != 0 && j < LENGTH_ENOUGH*/; j++){
			
			
			if (diff_all[j]>diff_max){
				max_i = j;
				diff_max = diff_all[j];
			}
		}

		int temp = 0;
		temp= diff_all[i];                            //交换 diff_all,x,y,circle_all_r
		diff_all[i] = diff_all[max_i];
		diff_all[max_i] = temp;

		temp = x[i];
		x[i] = x[max_i];
		x[max_i] = temp;

		temp = y[i];
		y[i] = y[max_i];
		y[max_i] = temp;

		temp = circle_all_r[i];
		circle_all_r[i] = circle_all_r[max_i];
		circle_all_r[max_i] = temp;
	}

	//非极大值抑制
	for (int i = 0; i < posi; i++){
		for (int j = i+1; j < posi; j++){
			if (diff_all[j]){
				int distance_cen;
				int distance_r;
				distance_cen = (x[j] - x[i])*(x[j] - x[i]) + (y[j] - y[i])*(y[j] - y[i]);
				distance_r = circle_all_r[i] - circle_all_r[j];
				if (distance_cen < CIRCLE_CENTRE_SCALE_square && (distance_r < CIRCLE_R_SCALE || distance_r> -CIRCLE_R_SCALE)){
					diff_all[j] = 0;
					x[j] = 0;
				}
			}
			
		}
	}

	//计算同心圆
	for (int i = 0; i < posi; i++){
		len[i] = 1;
		for (int j = i + 1; j < posi; j++){
			if (diff_all[j]){
				if (x[i] == x[j] && y[i] == y[j]){
					diff_all[j] = 0;
					x[j] = 0;
					len[i]++;
				}
			}

		}
	}




#ifdef IMG_SHOW

	for (int count = 0; count<posi/*x[count] != 0*/; count++){
		if (diff_all[count]){
			colorImg.data[3 * (x[count] * col + y[count]) + 1] = 0;
			colorImg.data[3 * (x[count] * col + y[count]) + 2] = 0;
			colorImg.data[3 * (x[count] * col + y[count])] = 0;            //标记圆心 y[count]

		}
	}

	cout << "final print:" << endl;
	cout << max_i << "," << max_j << " max:" << max << endl;
	for (int count = 0; count<posi/*x[count] != 0*/; count++){
		if (diff_all[count]){
			cout << "x:" << x[count] << "  y:" << y[count] << "  r:" << circle_all_r[count] << "  len:" << len[count] << "  diff:" << diff_all[count] << "  /360:" << diff_all[count] / 360 << endl;
		}
	}

	namedWindow("CEN_new", CV_WINDOW_NORMAL);
	imshow("CEN_new", colorImg);
	waitKey();
#endif
	return MY_OK;
}
