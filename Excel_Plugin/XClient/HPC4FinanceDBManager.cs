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
using MySql.Data.MySqlClient;
using System.Configuration;
using XClient.Properties;

class HPC_FinanceDBManager
{
    Settings set = Settings.Default;
    
    /// <summary>
    /// Get available financial models
    /// </summary>
    /// <returns>List of Models</returns>
    public List<string> GetModels()
    {
        List<string> list = new List<string>();
        MySqlConnection connection = new MySqlConnection(set.mnConnectionString);
        MySqlDataReader reader;
        try
        {
            connection.Open();
            
            string queryString = "select distinct name from model";
            MySqlCommand command = new MySqlCommand(queryString);
            command.Connection = connection;
            reader = command.ExecuteReader();

            while (reader.Read())
            {
                list.Add(reader[0].ToString());
            }
            reader.Close();
        }
        catch (Exception)
        {
        }
        finally
        {
            connection.Close();
        }
        return list;
    }

    /// <summary>
    /// Get the path in the server of a corresponding model
    /// </summary>
    /// <param name="name">mane of the model</param>
    /// <returns>path</returns>
    public string GetPath(string name)
    {
        string path = null;
        MySqlConnection connection = new MySqlConnection(set.mnConnectionString);
        try
        {
            connection.Open();

            string queryString = "select path from model where name = '" + name + "'";
            MySqlCommand command = new MySqlCommand(queryString);
            command.Connection = connection;
            path = (string)command.ExecuteScalar();

        }
        catch (Exception ex)
        {
            throw ex;
        }
        finally
        {
            connection.Close();
        }
        return path;
    }

    /// <summary>
    /// Get list of parameters for a particular model
    /// </summary>
    /// <param name="name">model</param>
    /// <returns>comma seperated parameters</returns>
    public string GetParaList(string name)
    {
        string paraList = null;
        MySqlConnection connection = new MySqlConnection(set.mnConnectionString);
        try
        {
            connection.Open();

            string queryString = "select parameters from model where name = '" + name + "'";
            MySqlCommand command = new MySqlCommand(queryString);
            command.Connection = connection;
            paraList = (string)command.ExecuteScalar();

        }
        catch (Exception)
        {
        }
        finally
        {
            connection.Close();
        }
        return paraList;
    }

    /// <summary>
    /// Get parameter format foe a partucular model
    /// </summary>
    /// <param name="name">model</param>
    /// <returns>comma seperated prameter types</returns>
    public string GetParaTypes(string name)
    {
        string paraList = null;
        MySqlConnection connection = new MySqlConnection(set.mnConnectionString);
        try
        {
            connection.Open();

            string queryString = "select paratype from model where name = '" + name + "'";
            MySqlCommand command = new MySqlCommand(queryString);
            command.Connection = connection;
            paraList = (string)command.ExecuteScalar();

        }
        catch (Exception)
        {
        }
        finally
        {
            connection.Close();
        }
        return paraList;
    }

}


