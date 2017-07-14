#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
//#include<cmath>
#include<stack>
using namespace std;

struct KdTree{
	vector<double> root;
	int split;
	KdTree *parent;
	KdTree *left;
	KdTree *right;
	KdTree(){parent=left=right=NULL;}
	bool isEmpty(){
		return root.empty();
	}
	bool isLeaf(){
		return (!root.empty())&&right==NULL&&left==NULL;
	}
	bool isRoot(){
		return (!isEmpty())&&parent==NULL;
	}
	bool isLeft(){
		return parent->left->root==root;
	}
	bool isRight(){
		return parent->right->root==root;
	}
};

template <typename T>
T findMiddleValue(vector<T> vec){
	sort(vec.begin(),vec.end());
	auto pos=vec.size()/2;
	return vec[pos];
}

//template <typename T>
vector<vector<double>> Transpose(vector<vector<double>> Matrix){
	unsigned row=Matrix.size();
	unsigned col=Matrix[0].size();
	vector<vector<double>> Tran(col,vector<double>(row,0));
	for(unsigned i=0;i<col;i++){
		for(unsigned j=0;j<row;j++){
			Tran[i][j]=Matrix[j][i];
		}
	}
	return Tran;
}

//构建kd树
void buildKdTree(KdTree* tree, vector<vector<double>> data,unsigned depth){
	unsigned datanum=data.size();
	if(datanum==0) {return;}
	unsigned k= data[0].size();
	int splitAttribute=depth % k; //选择切分维度
	if(datanum==1){
		tree->root=data[0];
		tree->split=splitAttribute;
		return;
	}
	
	vector<vector<double>> transdata=Transpose(data);
	
	vector<double> splitAttributeValues=transdata[splitAttribute];
	double splitValue=findMiddleValue(splitAttributeValues);
	vector<vector<double> > subset1;
	vector<vector<double> > subset2;

	
	//样本维度
	for(unsigned i=0;i<datanum;++i){
		if(splitAttributeValues[i]==splitValue&&tree->root.empty()){
			tree->root=data[i];
		    tree->split=splitAttribute;
		}
			
		else{
			if(splitAttributeValues[i]<splitValue)
				subset1.push_back(data[i]);
			else 
				subset2.push_back(data[i]);
		}
	}
	
		tree->left= new KdTree;
		tree->left->parent=tree;
		tree->right= new KdTree;
		tree->right->parent=tree;
		buildKdTree(tree->left,subset1,depth+1);
		buildKdTree(tree->right,subset2,depth+1);
	

}

double Distance(vector<double> x1,vector<double> x2,int method){
	if(x1.size()!=x2.size()) {
		cout<<"wrong!!!"<<endl;
		exit(1);
	}
	switch(method){
	case 0:{
		  //欧氏距离
		  double res=0;
		  for(int i=0;i<x2.size();i++){
			  res+=(x1[i]-x2[i])*(x1[i]-x2[i]);
		  }
		  return sqrt(res);
		   }
	case 1:{
		  //曼哈顿距离
		  double res=0;
		  for(int i=0;i<x2.size();i++){
			  res+=fabs(x1[i]-x2[i]);
		  }
		  return res;
		   }
	default:{
		      cout<<"invalid method!!"<<endl;
			  return -1;
			}

	}
}

vector<double> search(vector<double> test,KdTree *tree){
	double dist=0,min_dist=0;
	vector<double> nearest;
	int s;
	stack<KdTree*> back_point;
	KdTree *Kd_point=tree;
	nearest=Kd_point->root;
	min_dist=Distance(nearest,test,0);
	while(Kd_point!=NULL){
		back_point.push(Kd_point);
		if(Distance(nearest,test,0)>Distance(Kd_point->root,test,0)){
			nearest=Kd_point->root;
			min_dist=Distance(Kd_point->root,test,0);
		}
		s=Kd_point->split;
		if(test[s]<=Kd_point->root[s])
			Kd_point=Kd_point->left;
		else
			Kd_point=Kd_point->right;
		
	}
	nearest=back_point.top()->root;
	back_point.pop();
	min_dist=Distance(test,nearest,0);
	while(!back_point.empty()){
		KdTree *temp=back_point.top();
		back_point.pop();
		s=temp->split;
		if(Distance(test,temp->root,0)<min_dist){
			if(test[s]<=temp->root[s])
				temp=temp->right;
			else
				temp=temp->left;
			back_point.push(temp);
		}
		if(Distance(test,nearest,0)>Distance(temp->root,test,0)){
				nearest=temp->root;
				min_dist=Distance(temp->root,test,0);
			}
		}
	
	return nearest;
}

double data[6][2]={{2,3},{5,4},{9,6},{4,7},{8,1},{7,2}};

void main(){
	vector<vector<double>> train(6,vector<double>(2,0));
	vector<double> test;
	test.push_back(2);
	test.push_back(4.5);
	for(int i=0;i<6;i++){
		for(int j=0;j<2;j++){
			train[i][j]=data[i][j];
		}
	}
	KdTree *kdTree=new KdTree;
	buildKdTree(kdTree,train,0);
	vector<double> KNNneighbour=search(test,kdTree);
	vector<double>::iterator beg=KNNneighbour.begin();
	while(beg!=KNNneighbour.end()) cout<<*beg++<<",";
	cout<<Distance(KNNneighbour,test,0)<<endl;
	cout<<endl;
}
