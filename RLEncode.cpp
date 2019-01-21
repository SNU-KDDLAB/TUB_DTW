#include <vector>
#include <cassert>
#include <cmath>
#include <cstdlib>
bool *RLEncode(std::vector<double> &s, std::vector<double> &encoded, int depth = 1) {
	encoded.clear();

	bool *temp = new bool[s.size()];
	int count = 0;
	int k = 0;

    if(depth == 1) {
	    for (std::vector<double>::size_type i = 0; i < s.size()-1; i++) {
        	if (s[i] == 0 && k > 0) {
	        	count++;
	    	    if (i == s.size() - 2) {
        			encoded.push_back((double)count);
	    		    count = 0;
		        	temp[k++] = true;
    	    	}
	        }
        	else {
	        	if (count > 0) {
	    	    	encoded.push_back((double)count);
    				temp[k++] = true;
	    	    }
		    	count = 0;
	    	    encoded.push_back(s[i]);
    			temp[k++] = false;
    	    }
        }
        encoded.push_back(s[s.size()-1]);
        temp[k] = false;
	}

    else {
        int nonzero = 0;
        for(std::vector<double>::iterator it = s.begin(); it != s.end(); it++) {
            if(*it != 0) nonzero++;
        }

        int sparsity = s.size() / nonzero;
        int avg_length = (sparsity-1)/depth;

    	for (std::vector<double>::size_type i = 0; i < s.size(); i++) {
    	    if (s[i] == 0 && k > 0) {
	        	count++;
    		    if (i == s.size() - 2) {
    			    encoded.push_back((double)count);
    			    count = 0;
    	    		temp[k++] = true;
        		}
    	   	}
   		    else {
	   	    	if (count > 0) {
	   	    	    if(count <= avg_length) {
     	   	    		encoded.push_back((double)count);
    		   		    temp[k++] = true;
                    }
                    else {
                        int basic_length = count / depth;
                        int *run_length = new int[depth];
                        int remainder = count % depth;

                        for(int index = 0; index < depth; index++) {
                            if(index < remainder) run_length[index] = basic_length + 1;
                            else run_length[index] = basic_length;
    
                            encoded.push_back((double)run_length[index]);
                            temp[k++] = true;
                        }
                    }
    	    	}
	        	count = 0;
    		    encoded.push_back(s[i]);
    			temp[k++] = false;
	    	}
    	}

	}

	bool *run = new bool[encoded.size()];

	for (unsigned int i = 0; i < encoded.size(); i++) {
		run[i] = temp[i];
	}

	delete[] temp;

	return run;
}


int *RL_BDTW(std::vector<double> &s, std::vector<double> &encoded) {
	/*
	==== Input ====
	is_dense : 1 if already encoded(twitter data)

	==== Output ====
	array with values
	

	*/

	encoded.clear();

	int *temp = new int[s.size()];
	int count = 0;
	int k = 1;

	double current = s[1];
	count = 0;

	encoded.push_back(s[0]);
	temp[0] = 1;
	
	for (std::vector<double>::size_type i = 1; i < s.size()-1; i++) {
    	if (s[i] == current) {
    		count++;
	    	if (i == s.size()-2) {
        		encoded.push_back(current);
	    		temp[k++] = count;
    	    }
	    }
        else {
	        encoded.push_back(current);
    		temp[k++] = count;

    		if(i == s.size()-2) {
    			encoded.push_back(s[i]);
    			temp[k++] = 1;
    		}
	   	   	current = s[i];
	        count = 1;
	    }
	}

	encoded.push_back(s[s.size()-1]);
	temp[k++] = 1;

	assert(encoded.size() == k);


	int *run = new int[encoded.size()];

	for (unsigned int i = 0; i < encoded.size(); i++) {
		run[i] = temp[i];
	}

	delete[] temp;

	return run;
}

bool *RL_partial(std::vector<double> &s, std::vector<double> &encoded) {
	encoded.clear();

	bool *temp = new bool[s.size()];
	int count = 0;
	int k = 0;
	
	// state = 1: first zero encoded
	// state = 0: otherwise
	int state = 0;

	for (std::vector<double>::size_type i = 0; i < s.size()-1; i++) {
      	if (s[i] == 0 && k > 0) {
			if(state != 1) {
				state = 1;
				encoded.push_back(1.0);
				temp[k++] = true;
				continue;
			}

	       	count++;
	   	    if (i == s.size() - 2) {
       			encoded.push_back((double)count);
	   		    count = 0;
	        	temp[k++] = true;
        	}
	    }
       	else {
	       	if (count > 0) {
	   	    	encoded.push_back((double)count);
    			temp[k++] = true;
	   	    }
	    	count = 0;
	   	    encoded.push_back(s[i]);
    		temp[k++] = false;
    		state = 0;
        }
    }
    encoded.push_back(s[s.size()-1]);
    temp[k] = false;
	bool *run = new bool[encoded.size()];

	for (unsigned int i = 0; i < encoded.size(); i++) {
		run[i] = temp[i];
	}

	delete[] temp;

	return run;
}


