/***************************************************************************
 *   Copyright (C) 2016 by pgRouting developers                            *
 *   project@pgrouting.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License t &or more details.                        *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef SRC_EXPORT2DB_H_
#define SRC_EXPORT2DB_H_

#include <libpq-fe.h>
#include <map>
#include <vector>
#include <string>

#include "./Node.h"
#include "./Way.h"
#include "./Relation.h"
#include "./Type.h"
#include "./Class.h"
#include "./Configuration.h"
#include "./prog_options.h"

namespace osm2pgr {

/**
 * This class connects to a postgresql database. For using this class,
 * you also need to install postgis and pgrouting
 */

class Export2DB {
 public:
     /**
      * Constructor 
      * @param vm variable map holding the configuration
      *
      */
     explicit Export2DB(const po::variables_map &vm);

     /**
      * Destructor
      * closes the connection to the database
      */
     ~Export2DB();

     //! connects to database
     int connect();
     bool has_postGIS() const;

     //! creates needed tables and geometries
     void createTables() const;
     void createTempTables() const;
     //! exports nodes to the database
     void prepareExportNodes(const std::string nodes_columns) const;
     void exportNodes(const std::map<int64_t, Node>& nodes) const;
     void processSectionExportNodes(const std::string nodes_columns) const;

     //! exports ways to the database
     void exportTags(
             const std::map<int64_t, Way> &ways,
             const Configuration &config) const;
     void exportRelations(
             const std::vector<Relation> &relations,
             const Configuration &config) const;
     void exportRelationsWays(
             const std::vector<Relation> &relations,
             const Configuration &config) const;
     void exportTypes(const std::map<std::string, Type>& types) const;
     void exportClasses(const std::map<std::string, Type>& types) const;
     void exportWays(
             const std::map<int64_t, Way> &ways,
             const Configuration &config) const;

     //! Be careful! It deletes the created tables!
     void dropTables() const;
     void dropTempTables() const;
     void createFKeys();

 private:
     //! to use with creating the ways
     void prepare_table(const std::string &ways_columns) const;
     void process_section(const std::string &ways_columns) const;

     void dropTempTable(
             const std::string &table) const;
     bool createTempTable(
             const std::string &sql,
             const std::string &table) const;
     void dropTable(const std::string &table) const;
     bool createTempTable(
             const std::string &sql,
             const std::string &table);
     bool createTable(
             const std::string &sql,
             const std::string &table,
             const std::string &constraint = std::string("")) const;
     void addTempGeometry(
             const std::string &table,
             const std::string &geometry_type) const;
     void addGeometry(
             const std::string &schema,
             const std::string &table,
             const std::string &geometry_type) const;
     void create_gindex(
             const std::string &index,
             const std::string &table) const;
     void create_idindex(
             const std::string &colname,
             const std::string &table) const;
     inline std::string full_table_name(const std::string &table) const {
         return tables_prefix + table + tables_suffix;
     }
     inline std::string addSchema(const std::string &table) const {
         return  (default_tables_schema() == "" ? ""
                 : default_tables_schema() + ".") + table;
     }
     inline std::string default_tables_schema() const {
         return tables_schema;
     }
     void fill_vertices_table(
             const std::string &table,
             const std::string &vertices_tab) const;
     void fill_source_target(
             const std::string &table,
             const std::string &vertices_tab) const;

 private:
     PGconn *mycon;
     std::string conninf;
     std::string tables_schema;
     std::string tables_prefix;
     std::string tables_suffix;

     // create table query constants
     std::string create_classes;
     std::string create_nodes;
     std::string create_ways;
     std::string create_relations;
     std::string create_relations_ways;
     std::string create_way_tag;
     std::string create_types;
     std::string create_vertices;
};
}  // namespace osm2pgr

#endif  // SRC_EXPORT2DB_H_
