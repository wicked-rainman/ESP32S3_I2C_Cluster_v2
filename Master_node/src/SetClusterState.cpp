#include "SetClusterState.h"
void IRAM_ATTR SetClusterState() {
    if(ClusterState=='R') ClusterState='S';
    else ClusterState='R';
}