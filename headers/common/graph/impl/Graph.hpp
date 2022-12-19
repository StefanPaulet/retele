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

    this->_nodeCount = jsonData.getInt ( "nodeCount" );
    this->_edgeCount = jsonData.getInt ( "edgeCount" );


    this->_pNodeList = new Node * [ this->_nodeCount ];
    this->_pEdgeList = new Edge * [ this->_edgeCount ];

    for ( auto i = 0; i < this->_nodeCount; ++ i ) {
        this->_pNodeList [ i ] = new Node ( i );
    }

    for ( auto & e : jsonData.getArray ( "gasStations" ) ) {
        int gasStation = e.getJson().getInt ( "id" );
        this->_pNodeList [ gasStation ]->setGasStation();
    }

    int i = 0;
    for ( auto & e : jsonData.getArray ( "streets" ) ) {
        auto aux = e.getJson();

        uint16 firstNode   = aux.getInt ( "firstEndId" );
        uint16 secondNode  = aux.getInt ( "secondEndId" );
        uint8 speedLimit   = aux.getInt ( "speedLimit" );
        uint8 length       = aux.getInt ( "length" );
        std :: string name = aux.getString ( "name" );

        this->_pEdgeList [ i ] = new Edge ( i, this->_pNodeList [ firstNode ], this->_pNodeList [ secondNode ], speedLimit, length, std :: move ( name ) );
        this->_pNodeList [ firstNode ]->add_incident_street ( this->_pEdgeList [ i ] );
        this->_pNodeList [ secondNode ]->add_incident_street ( this->_pEdgeList [ i ] );

        i ++;
    }
}


auto constexpr Graph :: getStreet ( int const & streetId ) -> Edge * {

    return this->_pEdgeList [ streetId ];
}


auto constexpr Graph :: getEdgeCount () const -> uint16 {

    return this->_edgeCount;
}


auto constexpr Graph :: getNodeCount () const -> uint16 {

    return this->_nodeCount;
}


auto Graph :: bfsTraversal (
        uint16              streetId,
        const std :: function < bool ( Node const * ) > & predicate
) -> std :: list < Node const * > * {

    auto pNodeQueue = new std :: queue < Node const * >;
    auto pResultList = new std :: list < Node const * >;

    bool * visited = new bool [ this->_nodeCount ];

    int * distanceTraveled = new int [ this->_nodeCount ];
    bool resultFound = false;

    pNodeQueue->push ( this->_pEdgeList [ streetId ]->getEndNodes().first );
    visited [ this->_pEdgeList [ streetId ]->getEndNodes().first->getId() ] = true;

    pNodeQueue->push ( this->_pEdgeList [ streetId ]->getEndNodes().second );
    visited [ this->_pEdgeList [ streetId ]->getEndNodes().second->getId() ] = true;

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
