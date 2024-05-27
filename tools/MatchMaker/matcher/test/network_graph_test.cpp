#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "util/debug.hpp"
#include "network/network_graph.hpp"
#include "network/bidirectional_network_graph.hpp"

using namespace FMM;
using namespace FMM::CORE;
using namespace FMM::NETWORK;
using namespace FMM::MM;
using namespace std;

TEST_CASE( "Network graph is tested", "[network_graph]" ) {
  spdlog::set_level((spdlog::level::level_enum) 0);
  spdlog::set_pattern("[%l][%s:%-3#] %v");
  Network network("../data/network.gpkg");
  NetworkGraph ng(network);
  BidirectionalNetworkGraph bng(network);

  SECTION( "single_source_upperbound_dijkstra" ) {
    NodeID nid = 2;
    NodeIndex source = network.get_node_index(nid);
    double delta = 5.1;
    PredecessorMap pmap;
    DistanceMap dmap;
    ng.single_source_upperbound_dijkstra(source,delta,&pmap,&dmap);
    REQUIRE(dmap.at(network.get_node_index(4))==5.0);
    REQUIRE(pmap.at(network.get_node_index(4))==
            network.get_node_index(9));
    REQUIRE(dmap.find(network.get_node_index(3))==dmap.end());
  }

  SECTION( "get_edge_index" ) {
    REQUIRE(network.get_edge_id(ng.get_edge_index(
      network.get_node_index(11),network.get_node_index(12),1
    ))==20);
    REQUIRE(ng.get_edge_index(
      network.get_node_index(11),network.get_node_index(12),0.5
    ) == -1);
  }

  SECTION( "bidirectional_dijkstra" ) {
    NodeID source_id = 2;
    NodeID target_id = 3;
    NodeIndex source = network.get_node_index(source_id);
    NodeIndex target = network.get_node_index(target_id);
    std::vector<EdgeIndex> path1 = ng.shortest_path_dijkstra(source,target);
    std::vector<EdgeIndex> path2 = bng.shortest_path_bidirectional_dijkstra(
      source, target);
    REQUIRE_THAT(path1,Catch::Equals<EdgeIndex>(path2));
  }

  SECTION( "bidirectional_dijkstra_within_dist" ) {
    EdgeID edge_id = 2;
    double dist = 3;
    auto result = bng.search_edges_within_dist_ft_edge(
      edge_id, dist);
    std::cout<<"Edges found: ";
    for (auto item:result){
      std::cout<<item<<" ";
    }
    std::cout<<"\n";
  }

}
