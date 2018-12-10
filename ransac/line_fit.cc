#include <iostream>
#include <vector>

#include <random>
#include <algorithm>
#include <iterator>

#include <math.h>

#include <float.h>

using namespace std;

std::random_device rd;
std::mt19937 g(rd());

struct ModelParams {
	double m;
	double b;
};

typedef struct ModelParams ModelParams;

double mean(const std::vector<double> &points) {
	double sum=0.0;
	for (int i=0;i<points.size();i++)
		sum+=(double)points[i];

	return (sum/(double)points.size());
}

void fitModel(const std::vector<double> &xpoints, const std::vector<double> &ypoints, ModelParams &params) {
	double meanx=mean(xpoints);
	double meany=mean(ypoints);
	double sumtop=0.0;
	double sumbottom=0.0;
	double xminusmean=0.0;
	params.m=0.0;
	params.b=0.0;

	for (int i=0;i<xpoints.size();i++) {
		xminusmean=(double)xpoints[i]-meanx;
		sumtop+=(xminusmean)*((double)ypoints[i]-meany);
		sumbottom+=(xminusmean*xminusmean);
	}
	params.m=sumtop/sumbottom;
	params.b=meany-params.m*meanx;
	cout << "fit - m: " << params.m << " b: " << params.b << endl;
}

double pointError(const ModelParams &params, double x, double y) {
	double yhat=x*params.m+params.b;
	return (yhat - y);
}

double costFunc(const ModelParams &params, const std::vector<double> &xpoints, const std::vector<double> &ypoints) {
	double cost=0.0;
	double m=xpoints.size();
	double error=0.0;

	for (int i=0;i<m;i++) {
		error=pointError(params, xpoints[i], ypoints[i]);

		cost+=0.5*(1.0/(double)m)*(error*error);
	}
	return cost;
}

int main() {
	std::vector<double> xpoints={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,3.0};
	std::vector<double> ypoints={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,6.0};
	std::vector<double> subx;
	std::vector<double> suby;

	ModelParams params={0.0,0.0};
	ModelParams maybeParams={0.0,0.0};
	ModelParams betterParams={0.0,0.0};
	ModelParams bestParams={0.0,0.0};
	double bestScore=DBL_MAX;
	std::vector<int> idxs;
	std::vector<int> maybeInliers;
	std::vector<int> alsoInliers;
	std::vector<int> combined;

	idxs.resize(xpoints.size());
	for (int i=0;i<idxs.size();i++)
		idxs[i]=i;
	int iters=0;
	int k=10;
	int n=5;
	int d=3;
	double t=0.01;
	double err=0.0;

	double p=0.97;
	double w=0.8;
	

	k=(int)ceilf(log(1.0-p)/log(1.0-pow(w,(double)n)));
	cout << "k: " << k << endl;

	fitModel(xpoints, ypoints, params);
	cout << "cost: " << costFunc(params, xpoints, ypoints) << endl;

	while (iters < k) {
		std::shuffle(idxs.begin(), idxs.end(), g);

		maybeInliers.assign(idxs.begin(), idxs.begin()+n);
		alsoInliers.clear();
		subx.clear();
		suby.clear();
		cout << "maybe: ";
		for (int i=0;i<maybeInliers.size();i++) {
			cout << maybeInliers[i] << " ";
			subx.push_back(xpoints[maybeInliers[i]]);
			suby.push_back(ypoints[maybeInliers[i]]);
		}
		cout << endl;

		fitModel(subx, suby, maybeParams);
		cout << "others: ";
		for (int i=n;i<idxs.size();i++) {
			cout << idxs[i] << " ";
			err=pointError(maybeParams, xpoints[idxs[i]], ypoints[idxs[i]]);
			//cout << err*err << " ";
			if (err*err < t)
				alsoInliers.push_back(idxs[i]);
		}
		cout << endl;
		cout << "alsoInliers.size(): " << alsoInliers.size() << ", d: " << d << endl;
		if (alsoInliers.size() >= d) {
			combined.clear();
			combined.reserve( maybeInliers.size() + alsoInliers.size() );
			combined.insert(combined.end(), maybeInliers.begin(), maybeInliers.end());
			combined.insert(combined.end(), alsoInliers.begin(), alsoInliers.end());
			subx.clear();
			suby.clear();

			for (int i=0;i<combined.size();i++) {
				subx.push_back(xpoints[combined[i]]);
				suby.push_back(ypoints[combined[i]]);
			}

			fitModel(subx, suby, betterParams);
			err=costFunc(betterParams, subx, suby);
			cout << "err: " << err << endl;
			if (err < bestScore) {
				bestScore=err;
				bestParams=betterParams;
				if (err==0.0)
					break;
			}
		}	
		iters++;
	}	
	cout << "best fit - m: " << bestParams.m << " b: " << bestParams.b << endl;
	return 0;
}
