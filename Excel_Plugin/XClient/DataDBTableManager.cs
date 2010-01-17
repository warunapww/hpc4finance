/*
 Copyright (c) 2008 by contributors:

 * Damitha Premadasa
 * Nilendra Weerasinghe
 * Thilina Dampahala
 * Waruna Ranasinghe - (http://warunapw.blogspot.com)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using MySql.Data.MySqlClient;
using System.Configuration;
using XClient.Properties;

class DataDBTableManager
{
    Settings set = Settings.Default;

    /// <summary>
    /// Return all tables in the database 
    /// </summary>
    /// <returns>List of strings</returns>
    public List<string> GetAllTables()
    {
        DataTable tables = null;
        List<string> tableList = new List<string>();
        DataTableReader r = null;
        MySqlConnection connection = new MySqlConnection(set.dataConnectionString);
        

        try
        {
            connection.Open();
            tables = connection.GetSchema("Tables");
            r = tables.CreateDataReader();

            while (r.Read())
            {
                tableList.Add(r[2].ToString());
            }
            r.Close();
        }
        catch (Exception)
        {
        }
        finally
        {
            connection.Close();
            
        }
        return tableList;
    }


    /// <summary>
    /// Create a new data table
    /// </summary>
    /// <param name="tableName">Table name</param>
    public void CreateTable(string tableName)
    {
        MySqlConnection connection = new MySqlConnection(set.dataConnectionString);
        try
        {
            string createQuery = "CREATE TABLE IF NOT EXISTS " + tableName.ToLower() + " (ID INT UNSIGNED AUTO_INCREMENT, Date Date, Value Double, PRIMARY KEY(ID))";
            MySqlCommand createCommand = new MySqlCommand(createQuery);
            createCommand.Connection = connection;
            connection.Open();
            createCommand.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
            throw ex;
        }
        finally
        {
            connection.Close();
        }
    }

    /// <summary>
    /// Delete a table from the database
    /// </summary>
    /// <param name="tableName">Table Name</param>
    public void DeleteTable(string tableName)
    {
        MySqlConnection connection = new MySqlConnection(set.dataConnectionString);
        try
        {
            string deleteQuery = "DROP TABLE " + tableName.ToLower();
            MySqlCommand deleteCommand = new MySqlCommand(deleteQuery);
            deleteCommand.Connection = connection;
            connection.Open();
            deleteCommand.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
            throw ex;
        }
        finally
        {
            connection.Close();
        }
    }
}

