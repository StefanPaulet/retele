//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_GRAPH_IMPL_HPP
#define CONCURRENT_SV_GRAPH_IMPL_HPP


Graph :: Graph() {

    cds :: String mapPathString = cds :: filesystem :: Path ( __COMMON_LIBS_PATH ).parent().parent() / "resources/map.json";
    auto mapPathStringLiteral = mapPathString.cStr();
    std :: stringstream iss;
    iss << std :: ifstream ( mapPathStringLiteral ).rdbuf();

    auto jsonData = cds :: json :: parseJson ( iss.str() );

    this->_node_count = ( uint16 ) jsonData.getInt ( "nodeCount" );
    this->_edge_count = ( uint16 ) jsonData.getInt ( "edgeCount" );


    this->_pNode_list = new Node * [ this->_node_count ];
    this->_pEdge_list = new Edge * [ this->_edge_count ];

    for ( uint16 i = 0; i < this->_node_count; ++ i ) {
        this->_pNode_list [ i ] = new Node ( i );
    }

    for ( auto & e : jsonData.getArray ( "gasStations" ) ) {
        int gasStation = e.getJson().getInt ( "id" );
        this->_pNode_list [ gasStation ]->setGasStation();
    }

    int i = 0;
    for ( auto & e : jsonData.getArray ( "streets" ) ) {
        auto entry = e.getJson();

        uint16 firstNode   = entry.getInt ( "firstEndId" );
        uint16 secondNode  = entry.getInt ( "secondEndId" );
        uint8 speedLimit   = entry.getInt ( "speedLimit" );
        uint8 length       = entry.getInt ( "length" );
        std :: string name = entry.getString ( "name" );

        this->_pEdge_list [ i ] = new Edge ( i, this->_pNode_list [ firstNode ], this->_pNode_list [ secondNode ], speedLimit, length, std :: move ( name ) );
        this->_pNode_list [ firstNode ]->add_incident_street ( this->_pEdge_list [ i ] );
        this->_pNode_list [ secondNode ]->add_incident_street ( this->_pEdge_list [ i ] );

        i ++;
    }
}


auto constexpr Graph :: getStreet ( int const & streetId ) -> Edge * {

    return this->_pEdge_list [ streetId ];
}


auto constexpr Graph :: getEdgeCount () const -> uint16 {

    return this->_edge_count;
}


auto constexpr Graph :: getNodeCount () const -> uint16 {

    return this->_node_count;
}


auto Graph :: bfsTraversal (
        uint16              streetId,
        const std :: function < bool ( Node const * ) > & predicate
) -> std :: list < Node const * > * {

    auto pNodeQueue = new std :: queue < Node const * >;
    auto pResultList = new std :: list < Node const * >;

    bool * visited = new bool [ this->_node_count ];

    int * distanceTraveled = new int [ this->_node_count ];
    bool resultFound = false;

    pNodeQueue->push ( this->_pEdge_list [ streetId ]->getEndNodes().first );
    visited [ this->_pEdge_list [ streetId ]->getEndNodes().first->getId() ] = true;

    pNodeQueue->push ( this->_pEdge_list [ streetId ]->getEndNodes().second );
    visited [ this->_pEdge_list [ streetId ]->getEndNodes().second->getId() ] = true;

    while ( ! pNodeQueue->empty() ) {
        auto currentNode = pNodeQueue->front();
        pNodeQueue->pop();

        if ( predicate ( currentNode ) ) {
            pResultList->push_back ( currentNode );
            resultFound = true;
        }

        if ( ! resultFound || distanceTraveled [ currentNode->getId() ] < 10 ) {
            for ( auto & e : * currentNode->getStreetList() ) {

                auto currentNeighbour = (
                        currentNode->getId() == e->getEndNodes().first->getId() ?
                        e->getEndNodes().second :
                        e->getEndNodes().first
                    );
                if ( ! visited [ currentNeighbour->getId() ] ) {
                    visited [ currentNeighbour->getId() ] = true;
                    distanceTraveled [ currentNeighbour->getId() ] = distanceTraveled [ currentNode->getId() ] + 1;
                    pNodeQueue->push ( currentNeighbour );
                }
            }
        }
    }

    delete[] visited;
    delete[] distanceTraveled;
    delete pNodeQueue;

    return pResultList;
}

#endif //CONCURRENT_SV_GRAPH_IMPL_HPP
