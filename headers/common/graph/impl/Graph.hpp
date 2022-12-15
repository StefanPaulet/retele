//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_GRAPH_IMPL_HPP
#define CONCURRENT_SV_GRAPH_IMPL_HPP

#define  __MAP_DATABASE_FILE "../resources/map.txt"      /* NOLINT(bugprone-reserved-identifier) */

Graph :: Graph() {


    std :: ifstream fin ( __MAP_DATABASE_FILE );

    int nodeCount, edgeCount;
    fin >> nodeCount >> edgeCount;

    this->_nodeCount = nodeCount;
    this->_edgeCount = edgeCount;


    this->_pNodeList = new Node * [ this->_nodeCount ];
    this->_pEdgeList = new Edge * [ this->_edgeCount ];

    for ( auto i = 0; i < this->_nodeCount; ++ i ) {
        this->_pNodeList [ i ] = new Node ( i );
    }

    int gasStationCount;
    fin >> gasStationCount;
    for ( int i = 0; i < gasStationCount; ++ i ) {
        int gasStation;
        fin >> gasStation;
        this->_pNodeList [ gasStation ]->setGasStation();
    }

    for ( int i = 0; i < this->_edgeCount; ++ i ) {
        int firstNode, secondNode, speedLimit, length;
        std :: string streetName;

        fin >> firstNode >> secondNode >> speedLimit >> length;
        std :: getline ( fin, streetName );

        this->_pEdgeList [ i ] = new Edge ( i, this->_pNodeList [ firstNode ], this->_pNodeList [ secondNode ], speedLimit, length, std :: move ( streetName ) );
        this->_pNodeList [ firstNode ]->add_incident_street ( this->_pEdgeList [ i ] );
        this->_pNodeList [ secondNode ]->add_incident_street ( this->_pEdgeList [ i ] );
    }
}


auto constexpr Graph :: getStreet (
        int const & streetId
    ) -> Edge * {

    return this->_pEdgeList [ streetId ];
}


auto constexpr Graph :: getEdgeCount () const -> uint16 {

    return this->_edgeCount;
}

#endif //CONCURRENT_SV_GRAPH_IMPL_HPP
