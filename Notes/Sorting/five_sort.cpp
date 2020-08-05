#include <utility>

using std::swap;

void five_sort(int list[]) {
	if (list[0] > list[1]) { swap(list[0],list[1]); }
	if (list[2] > list[3]) { swap(list[2],list[3]); }
    if (list[1] > list[3]) {
	    swap(list[1],list[3]);
	    swap(list[0],list[2]);
	}
    int a = list[0], b = list[1], c = list[2], d = list[3], e = list[4];
	if (e >= b) {
		if (e < d) {
			if (c >= b) {
				swap(list[3],list[4]); 
				if (c >= e) {swap(list[2],list[3]);}
            } else {
            	swap(list[0],list[2]);
            	swap(list[1],list[2]);
            	swap(list[3],list[4]);
            	if (c >= a) {swap(list[0],list[1]);}
            }
        } else {
        	if (c < b) {
        		swap(list[1],list[2]);
            	if (c < a) {swap(list[0],list[1]);}
            }
        }
    } else {
    	if (e < a) {
    		if (c >= a) {
    			swap(list[1],list[4]);
    			swap(list[0],list[1]);
    			swap(list[3],list[4]);
    			if (c >= b) {swap(list[2],list[3]);}
            } else {
            	swap(list[0],list[2]);
            	swap(list[1],list[4]);
            	swap(list[3],list[4]);
            	if (c >= e) {swap(list[0],list[1]);}
            }
        } else {
        	if (c >= e) {
        		swap(list[1],list[4]);
        		swap(list[3],list[4]);
        		if (c >= b) {swap(list[2],list[3]); }
            } else {
            	swap(list[0],list[2]);
            	swap(list[1],list[4]);
            	swap(list[1],list[2]);
            	swap(list[3],list[4]);
            	if (c >= a) {swap(list[0],list[1]);}
            }
        }
    }
}