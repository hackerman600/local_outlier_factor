#include <iostream>
#include <random>
#include <algorithm>
#include <map>


float return_average_k_distance(std::map<float,float>  myvectr, int k){
    std::map<int, float> top_k;
    std::vector<float> distances;
    std::vector<float> ordered_distances;
    float catchr = 0.f;
    int itter = 0;
    int index;


    if (myvectr.size() == k){
        for (std::map<float, float>::iterator st = myvectr.begin(); st != myvectr.end(); ++st){
                catchr += st->second;
        }
    }  

    else {
        
        for (std::map<float,float>::iterator vec = myvectr.begin(); vec != myvectr.end(); ++vec){
            //std::cout << "vec->first = " << vec->first << " vec->second = " << vec->second << std::endl;
            distances.push_back(vec->second);
        } 

        //std::cout << "\ndistances: \n"; 

        for (auto dist : distances){
            //std::cout << dist << std::endl;
        }

        //std::cout << "\nordered distances: \n";
        ordered_distances.push_back(0);
        for (auto dist : distances){ //{178,106,94,53,100,106,120,152}
            //std::cout << "dist = " << dist << std::endl;
            for (auto dis = ordered_distances.rbegin(); dis != ordered_distances.rend(); ++dis){//10000
                //std::cout << "*dis = " << *dis << std::endl;
                if (dist > *dis){
                    std::ptrdiff_t diff = std::distance(ordered_distances.rbegin(),dis);
                    
                    int index = ordered_distances.size() - diff;

                    ordered_distances.insert(ordered_distances.begin() + index,dist); 
                    break;
                } 
                else if (dist < *dis && dis == ordered_distances.rend()){
                    //std::cout << "rend reached." << std::endl;
                    ordered_distances.insert(ordered_distances.begin(),dist);
                    break;
                }
            }
        }

        ordered_distances.erase(ordered_distances.begin(), ordered_distances.begin() + 1);
        ordered_distances.erase(ordered_distances.begin() + k, ordered_distances.end());

        for (auto val : ordered_distances){
            //std::cout << val << std::endl;
            catchr += val;
        }


        



    }    
    return catchr / k;
}

std::map<float, float> calculate_local_density_scores(std::vector<int> data){
    std::map<float, float> lds;
    std::map<float,float> myvectr;  
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> deviation(0.01,0.1);
    int itter = 0;
    int k = 6;

    for (int itt = 0; itt < data.size() ; itt++){
        int itt2 = itt - k;
        for (itt2 ; itt2 <= itt + k ; itt2++){                       
             
            if (itt2 < 0 or itt == itt2 or itt2 >= data.size()){
                {};
            } 
            
            else{
                myvectr[data[itt2] + deviation(gen)] = 0;
            }
                       

        }


        //std::cout << "size of myvectr = " << myvectr.size() << "\n" << std::endl;    
        for (std::map<float,float>::iterator x = myvectr.begin(); x !=  myvectr.end(); ++x){
                int first = x->first;
                float distance = std::sqrt((data[itt] - first) * (data[itt] - first));
                myvectr[x->first] = distance;
        }

        

        for (std::map<float,float>::iterator x = myvectr.begin(); x !=  myvectr.end(); ++x){
                int first = x->first;
                //std::cout << "x->first = " << x->first << " x->second " << x->second << std::endl;
        } 
                    
        float average_k_distance = return_average_k_distance(myvectr, k); 
        float local_density = 1/average_k_distance;
        
        //std::cout << "average_k_distance = " << average_k_distance << std::endl;
        lds[data[itt]] = local_density;
        myvectr.clear();
        
                

    }

    return lds;

}

template <typename T>
T make_ascending(T data){
    T ascending = data;
    std::sort(ascending.begin(),ascending.end());

    
    return ascending;

}


int main(){

    std::vector<int> data;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<float> ascending_local_density_scores; 
    std::uniform_int_distribution<int> distribution(1,100);
    std::uniform_int_distribution<int> distribution2(200,400);
    std::string status;

    std::cout << "data is: \n" << std::endl;
    for (int i = 0; i < 15; i++){
        if (i < 10){
            data.push_back(distribution(gen));          
        } else {
            data.push_back(distribution2(gen));
        }
    }

        
    data.insert(data.end(),{500,486,666,532,480});

    for (auto d : data){
        std::cout << d << std::endl;
    }

    std::vector<int> ascending = make_ascending(data);
    std::cout << "\nascending = \n\n";

    for (auto val : ascending){
        std::cout << val << std::endl;
    }

    std::map<float, float> lds = calculate_local_density_scores(ascending);
    std::cout << "\nlocal density scores = \n" << std::endl;
    for (std::map<float, float>::iterator x = lds.begin(); x != lds.end(); x++){
        std::cout << std::round(x->first) << " - " << x->second << std::endl;
        ascending_local_density_scores.push_back(x->second);
    }

    std::vector<float> ascending_local_densitys = make_ascending(ascending_local_density_scores);
    
    std::cout << "\nascending_local_density_scores = \n" << std::endl;
    for (auto alds : ascending_local_densitys){
        if (alds < 0.01){
            status = "<<<<<<ANOMALY>>>>>>";
        } else {
            status = "normal";
        }
        std::cout << alds << "-" << status <<std::endl;
    }
    



   
    return 0;
}

