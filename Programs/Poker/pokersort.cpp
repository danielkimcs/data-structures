#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
using std::cout;
using std::swap;
using std::vector;
using std::endl;

enum Hand {straight_flush = 9,
four_of_a_kind = 8,
full_house = 7,
flush = 6,
straight = 5,
three_of_a_kind = 4,
two_pairs = 3,
one_pair = 2,
high_card = 1,
none = -1};
const long ENCODING_FACTOR = 10000000000;
const int THRESHOLD = 16;

void poker_sort(vector<int>& a);

void five_sort(int b[]) {
	for (int i = 1; i < 5; ++i) {
		int t = i;
		while (t > 0 && b[t]>b[t-1]) {
			swap(b[t],b[t-1]);
			t--;
		}
	}
}

long eval(int h) {
	int x = h; int cfl;
	int hr[5];
	for (int p = 0; p < 5; ++p){
		int xx = (x % 52 + 52) % 52; x /= 52;
		hr[p] = xx/4;
		if (p == 0) cfl = xx%4;
		else if (xx%4 != cfl) cfl = -1; 
	}
	five_sort(hr);
	bool fl = (cfl != -1);
	long encoding = hr[0];
	for (int i = 1; i < 5; ++i) encoding = encoding * 100 + hr[i];
	Hand ht = straight;
	int f_kind = hr[1];
	for (int i = 1; i < 5; ++i) {
		if (hr[i-1] != hr[i]+1) ht = none;
		if (i > 1 && i < 4 && hr[i-1] != hr[i]) f_kind = -1;
	}
	bool special_straight = false;
	if (hr[4] == 0 && hr[3] == 1 && hr[2] == 2 && hr[1] == 3 && hr[0] == 12) {
		ht = straight;
		special_straight = true;
		if (fl) return straight_flush*ENCODING_FACTOR+302010200;
	}
	if (ht == straight && fl) return straight_flush*ENCODING_FACTOR+encoding;
	if (hr[0] != f_kind && f_kind != hr[4]) f_kind = -1;
	if (f_kind != -1) return four_of_a_kind*ENCODING_FACTOR+f_kind*101010100+(f_kind == hr[0] ? hr[4] : hr[0]);
	if ((hr[0] == hr[1]) && (hr[1] != hr[2]) && (hr[2] == hr[3]) && (hr[3] == hr[4])) return full_house*ENCODING_FACTOR+hr[2]*101010000+hr[0]*101;
	else if ((hr[0] == hr[1]) && (hr[1] == hr[2]) && (hr[2] != hr[3]) && (hr[3] == hr[4])) return full_house*ENCODING_FACTOR+hr[0]*101010000+hr[3]*101;
	if (fl) return flush*ENCODING_FACTOR+encoding;
	if (ht == straight) {
		if (special_straight) return straight*ENCODING_FACTOR+302010200;
		else return straight*ENCODING_FACTOR+encoding;
	} 
	for (int i = 0; i < 3; ++i) {
		if ((hr[i] == hr[i+1]) && (hr[i+1] == hr[i+2])) {
			if (i == 0) return three_of_a_kind*ENCODING_FACTOR+encoding;//012
			if (i == 1) return three_of_a_kind*ENCODING_FACTOR+hr[0]*100+hr[1]*101010000+hr[4];//123
			if (i == 2) return three_of_a_kind*ENCODING_FACTOR+hr[0]*100+hr[1]+hr[2]*101010000;//234
		}
	}
	if ((hr[0] == hr[1]) && (hr[2] == hr[3])) return two_pairs*ENCODING_FACTOR+hr[0]*101000000+hr[2]*10100+hr[4];
	else if ((hr[0] == hr[1]) && (hr[3] == hr[4])) return two_pairs*ENCODING_FACTOR+hr[0]*101000000+hr[3]*10100+hr[2];
	else if ((hr[1] == hr[2]) && (hr[3] == hr[4])) return two_pairs*ENCODING_FACTOR+hr[1]*101000000+hr[3]*10100+hr[0];
	if (hr[0] == hr[1]) return one_pair*ENCODING_FACTOR+hr[0]*101000000+hr[2]*10000+hr[3]*100+hr[4];
	if (hr[1] == hr[2]) return one_pair*ENCODING_FACTOR+hr[1]*101000000+hr[0]*10000+hr[3]*100+hr[4];
	if (hr[2] == hr[3]) return one_pair*ENCODING_FACTOR+hr[2]*101000000+hr[0]*10000+hr[1]*100+hr[4];
	if (hr[3] == hr[4]) return one_pair*ENCODING_FACTOR+hr[3]*101000000+hr[0]*10000+hr[1]*100+hr[2];
	return high_card*ENCODING_FACTOR+encoding;
}

void insertion_sort(vector<int>& a,long b[], int lo, int hi) {
	for (int i = lo + 1; i <= hi; ++i) {
		int t = i;
		while (t > lo && b[t]<b[t-1]) {
			swap(a[t],a[t-1]);
			swap(b[t],b[t-1]);
			t--;
		}
	}
}

int qsPartition(vector<int>& a, long b[], int lo, int hi) {
    int med = (lo+hi)/2; long x;
    if (b[lo]<b[med]) {
    	if (b[med]<b[hi]) x = b[med];
    	else {
    		if (b[lo]<b[hi]) x = b[hi];
    		else x = b[lo];
    	}
    } else {
    	if (b[lo]<b[hi]) x = b[lo];
    	else {
    		if (b[med]<b[hi]) x = b[hi];
    		else x = b[med];
    	}
    }
    while (lo <= hi) {
        while (b[lo] < x) ++lo;
        while (b[hi] > x) --hi;
        if (lo <= hi) {
            swap(b[lo],b[hi]);
            swap(a[lo++],a[hi--]);
        }
    }
    return lo;
}

void chocolate_quicksort(vector<int>& a, long b[], int lo, int hi) {
	while (lo < hi) {
		if (hi - lo <= THRESHOLD) {
			insertion_sort(a,b,lo,hi);
			return;
		} else {
	        int m = qsPartition(a,b,lo,hi);
	        if (m - lo < hi - m) {
	            chocolate_quicksort(a, b, lo, m-1);
	            lo = m;
	        } else {
	            chocolate_quicksort(a, b, m, hi);
	            hi = m-1;
	        }
	    }
	}
    return;
}

void poker_sort(vector<int>& a) {
	long b[a.size()];
	for (int i = 0; i < a.size(); ++i) b[i] = eval(a[i]);
	chocolate_quicksort(a,b,0,a.size()-1);
}