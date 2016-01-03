#ifndef DBTYPESMAP_ORACLE
# define DBTYPESMAP_ORACLE

#ifndef OCCI_ORACLE
# include <occi.h>
#endif

#ifndef DBTYPES_ORACLE
# include "DbTypes.h"
#endif

void DbTypesMap(oracle::occi::Environment* envOCCI_);

#endif
