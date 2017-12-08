#include <boost/format.hpp>
#include <boost/graph/distributed/mpi_process_group.hpp>
#include <boost/graph/distributed/adjacency_list.hpp>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include "ranker.h"

#ifndef TYPES_H
#define TYPES_H


#include "types.h"

#endif 
//#include <boost/lexical_cast.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexProperties, EdgeProperties> Graph;

typedef  boost::graph_traits < Graph >::vertex_descriptor V;
typedef  boost::graph_traits < Graph >::edge_descriptor E;
typedef  boost::graph_traits < Graph >::vertex_iterator V_i;
typedef  boost::graph_traits < Graph >::edge_iterator E_i;
typedef boost::graph_traits < Graph >::out_edge_iterator EO_i;
typedef boost::graph_traits < Graph >::in_edge_iterator IO_i;


template<class G>
void print_graph(G &g) {
	typedef  typename boost::graph_traits < G >::vertex_iterator V_i;

	typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
	V_i vi;
	EO_i out, out_end;
	/* Vertices
	std::cout << "Vertices" << std::endl;
	for (vi = vertices(g).first;
                 vi != vertices(g).second; ++vi) {
		std::cout << g[*vi].id << ", ";
    }
    std::cout << std::endl;
	*/
    //Edges

	std::cout << "Edges" << std::endl;
    for (vi = boost::vertices(g).first;
            vi != boost::vertices(g).second; ++vi) {
        std::cout << g[*vi].id << ": " << " out_edges_of(" << g[(*vi)].id << "):";
        for (boost::tie(out, out_end) = out_edges(*vi, g);
                out != out_end; ++out) {
            std::cout << ' '
                << g[*out].id << "=("
                << g[*out].source << ", "
                << g[*out].target << ") = "
                << g[*out].weight <<"\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}



bool get_edge(Graph &g,
        std::map<long int, Graph::vertex_descriptor>& id_to_V,
        //std::map<long int, GGraph::edge_descriptor>& id_to_E,
        long int eid, long int source, long int target, double cost) {
	EO_i out, out_end;
	if (id_to_V.find(source) == id_to_V.end() || id_to_V.find(target) == id_to_V.end()) {
                return false;
        }
        else {
                for (boost::tie(out, out_end) = out_edges(id_to_V[source], g);
                        out != out_end; ++out) {
                        if (g[*out].target == target && g[*out].id == eid && g[*out].weight == cost) {
                                return true;
                        }
                }
        }

        return false;



}

/*
Adds an edge to graph g and updates its maps accordingly
*/

void add_edge_to_graph(Graph& g, 
	std::map<long int, Graph::vertex_descriptor>& id_to_V,
	std::map<long int, Graph::edge_descriptor>& id_to_E,
	long int eid, long int source,long int target, double weight, int level) {

	if(get_edge(g, id_to_V, 
		eid,
		source, target, weight))
		return;

	std::pair<Graph::edge_descriptor, bool> p;
	if (id_to_V.find(source) == id_to_V.end()) {
		//std::cout << "Adding vertex " <<  source << std::endl;
		//std::cout << "x: " << x_1 << ", y: " << y_1 << std::endl;
		id_to_V[source] = boost::add_vertex(g);
		g[id_to_V[source]].id = source;
	}
	if (id_to_V.find(target) == id_to_V.end()) {
		//std::cout << "Adding vertex " <<  target << std::endl;
		id_to_V[target] = boost::add_vertex(g);
		g[id_to_V[target]].id = target;

		//std::cout << "x: " << g[id_to_V[target]].x << ", y: " << g[id_to_V[target]].y << std::endl;
	}
	if (weight >= 0.00000) {
		
		p = boost::add_edge(id_to_V[source], id_to_V[target], g);
		g[p.first].weight = weight;
		g[p.first].source = source;
		g[p.first].target = target;
		g[p.first].idx = id_to_E.size();
		id_to_E[g[p.first].idx] = p.first;
		g[p.first].id = eid;
	}

	//std::cout << "added :)" << std::endl;

}




int construct_graph_from_file(std::string file_name, const char delimiter, Graph &g, std::map<long int,
	Graph::vertex_descriptor>& id_to_V,
	std::map<long int, Graph::edge_descriptor>& id_to_E) {
	std::ifstream file(("./data/"+file_name+".csv").c_str()); // pass file name as argment
	std::string linebuffer;
	long int eid, source, target;
	double cost, reverse_cost;
	long int edge_count = 0;
	std::pair<Graph::edge_descriptor, bool> p;
	while (file && getline(file, linebuffer)){
		if (linebuffer.length() < 1)continue;
		else {
			std::vector<std::string> result;
			std::stringstream ss(linebuffer);
			std::string token;
			while (std::getline(ss, token, delimiter)) {
			    result.push_back(token);
			}
			if (result.size() < 4)
				continue;
			/*
			eid = boost::lexical_cast<long int>(result[0].c_str());
			source = boost::lexical_cast<long int>(result[1].c_str());
			std::cout << "s " << source << std::endl;
			target = boost::lexical_cast<long int>(result[2].c_str());
			cost = boost::lexical_cast<double>(result[3].c_str());
			*/
			
			//std::cout << "s " << result.size() << std::endl;
			
			eid = std::atol(result[0].c_str());
			source = std::atol(result[1].c_str());
			target = std::atol(result[2].c_str());
			cost = std::atof(result[3].c_str());

			if (result.size() == 5)
				reverse_cost = std::atof(result[4].c_str());
				//reverse_cost = boost::lexical_cast<double>(result[4].c_str());
				
			else
				reverse_cost = -1.000;

			
			if (id_to_V.find(source) == id_to_V.end()) {
				id_to_V[source] = boost::add_vertex(g);
				g[id_to_V[source]].id = source;
			}
			if (id_to_V.find(target) == id_to_V.end()) {
				id_to_V[target] = boost::add_vertex(g);
				g[id_to_V[target]].id = target;
			}
			if (cost >= 0.00000) {
				
				p = boost::add_edge(id_to_V[source], id_to_V[target], g);
				g[p.first].weight = cost;
				g[p.first].source = source;
				g[p.first].target = target;
				id_to_E[edge_count] = p.first;
				g[p.first].idx = edge_count++;
				g[p.first].id = eid;
			}
			if (reverse_cost >= 0.00000) {
				p = boost::add_edge(id_to_V[target], id_to_V[source], g);
				g[p.first].weight = reverse_cost;
				g[p.first].source = target;
				g[p.first].target = source;
				id_to_E[edge_count] = p.first;
				g[p.first].idx = edge_count++;
				g[p.first].id = eid; 
			}
			
		}

	}
	return 1;
	
}







int get_levels(std::vector<double> values, int num_levels, std::vector<int>& level) {
    int bucket_size = 100/num_levels, interval_size, prev;
	std::vector<double> caps;

	for (int i = 0; i <= num_levels; ++i) {
		//caps.push_back(quantile(values, (i*bucket_size*1.000)/100));
		caps.insert(caps.begin(), quantile(values, (i*bucket_size*1.000)/100)); 
	}
	//Adding 1 to the highest betweenness value
	caps[0] += 1;
	//Adding -1 to the lowest betweenness value
	caps[caps.size()-1] -= 1;


	/*
	std::cout << "Intervals" << std::endl;
	for (int i = 0; i < caps.size(); ++i) {
		std::cout << caps[i] << ", ";
	}
	*/
	
	for (int i = 0; i < values.size(); ++i) {
		//std::cout << "b value: " << values[i] << std::endl;
		/* gets the bucket to which the value belongs
		TODO: convert this to binary search */
		for (int k = 0; k < caps.size()-1; k++) {
			if ((values[i] - caps[k+1] > 0.0000000001)
				&& (values[i] - caps[k] <= 0.0000000001)) {
				//std::cout << "value: " << k << std::endl;
				//std::cout << "caps[k]: " << caps[k] << ", "
				//<< "caps[k+1]: " << caps[k+1] << std::endl;
				level.push_back(k+1);
				break;
			}
		}
	}
	return 1;
}




int get_levels(std::vector<int> values, int num_levels, std::vector<int>& level) {
    int bucket_size = 100/num_levels, interval_size, prev;
	std::vector<int> caps;

	for (int i = 0; i <= num_levels; ++i) {
		//caps.push_back(quantile(values, (i*bucket_size*1.000)/100));
		caps.insert(caps.begin(), quantile(values, (i*bucket_size*1.000)/100)); 
	}
	//Adding 1 to the highest betweenness value
	caps[0] += 1;
	//Adding -1 to the lowest betweenness value
	caps[caps.size()-1] -= 1;


	/*
	std::cout << "Intervals" << std::endl;
	for (int i = 0; i < caps.size(); ++i) {
		std::cout << caps[i] << ", ";
	}
	*/
	
	for (int i = 0; i < values.size(); ++i) {
		//std::cout << "b value: " << values[i] << std::endl;
		/* gets the bucket to which the value belongs
		TODO: convert this to binary search */
		for (int k = 0; k < caps.size()-1; k++) {
			if ((values[i] - caps[k+1] > 0)
				&& (values[i] - caps[k] <= 0)) {
				//std::cout << "value: " << k << std::endl;
				//std::cout << "caps[k]: " << caps[k] << ", "
				//<< "caps[k+1]: " << caps[k+1] << std::endl;
				level.push_back(k+1);
				break;
			}
		}
	}
	return 1;
}

void get_equal_interval_levels(std::vector<int> values, 
	int num_levels, std::vector<int>& level) {
    int bucket_size = values.size()/num_levels;
    //std::cout << "bucket_size: " << bucket_size << std::endl;
    level.resize(values.size());
    int curr_level;
    std::vector<std::pair<int,int> > value_index;
    for (int i = 0; i < values.size(); ++i) {
    	value_index.push_back(std::make_pair(values[i], i));
    }
    sort(value_index.begin(), value_index.end());
    std::reverse(value_index.begin(), value_index.end());
    for (int i = 0; i < value_index.size(); ++i) {
    	//std::cout << "i: " << i << std::endl;
    	curr_level = (i/bucket_size)+1;
    	level[value_index[i].second] = curr_level > num_levels ? num_levels : curr_level;
    	//std::cout << "level: " << level[value_index[i].second] << std::endl;
    }
}

void get_equal_interval_levels(std::map<long int, long int> values, 
	int num_levels, std::map<long int, long int>& level) {
    int bucket_size = values.size()/num_levels;
    //std::cout << "bucket_size: " << bucket_size << std::endl;
    int curr_level;
    std::vector<std::pair<int,int> > value_index;
    std::map<long int, long int>::iterator it1, it2;
    for (it1 = values.begin(); it1 != values.end(); ++it1) {
    	value_index.push_back(std::make_pair(it1->second, it1->first));
    }
    sort(value_index.begin(), value_index.end());
    std::reverse(value_index.begin(), value_index.end());
    for (int i = 0; i < value_index.size(); ++i) {
    	//std::cout << "i: " << i << std::endl;
    	curr_level = (i/bucket_size)+1;
    	level[value_index[i].second] = curr_level > num_levels ? num_levels : curr_level;
    	//std::cout << "level: " << level[value_index[i].second] << std::endl;
    }
}

int get_random_sources(Graph &g, std::set<V>& indexes) {
	size_t max_index = num_vertices(g);
	int random_index;
	while (indexes.size() < sqrt(max_index)) {
		indexes.insert(rand() % max_index);
	}
	return 1;
}

int dump_to_file_group_by_id(const Graph &g, std::map<long int, Graph::edge_descriptor>& id_to_E,
	const std::vector<long int> edge_centrality, int num_levels,
	std::string output_file, const char delimiter) {
	std::ofstream myfile;
	std::map<long int, long int> edge_id_betweenness;
	std::map<long int, long int> edge_id_levels;
	std::map<long int, long int>::iterator it;
	long int temp;

	for (int i = 0; i < edge_centrality.size(); ++i) {
		std::cout << "id: " << g[id_to_E[i]].id 
		<< ", b: " << edge_centrality[i] << std::endl;

		if (edge_id_betweenness.find(g[id_to_E[i]].id) 
			!= edge_id_betweenness.end()) {
			temp = edge_id_betweenness[g[id_to_E[i]].id];
			edge_id_betweenness[g[id_to_E[i]].id] = 
			std::max(temp, edge_centrality[i]);
		}
		else
			edge_id_betweenness[g[id_to_E[i]].id] = edge_centrality[i];
    }
	get_equal_interval_levels(edge_id_betweenness, num_levels, edge_id_levels);
	//get_levels(edge_centrality, num_levels, level);
	assert(edge_id_betweenness.size() == edge_id_levels.size());
    myfile.open (("./data/"+output_file+"_betweenness.csv").c_str());

    for (it = edge_id_levels.begin(); it != edge_id_levels.end(); ++it) {
    	myfile << it->first << delimiter 
    	<< it->second << std::endl;
    	//<< comp_init << std::endl;
    }
    myfile.close();
    return 1;
}