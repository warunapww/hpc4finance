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
using XClient.Properties;


class DataWriter
{
    Settings set = Settings.Default;

    /// <summary>
    /// Committing Data To Database
    /// </summary>
    /// <param name="entityList">Entity List</param>
    /// <param name="tableName">Table Name</param>
    public void WriteData(List<DataEntity> entityList, string tableName)
    {
        MySqlConnection connection = new MySqlConnection(set.dataConnectionString);    
        try
        {
            entityList = GetUpdateList(entityList, tableName);
            connection.Open();
            foreach (DataEntity i in entityList)
            {
                string queryString = "INSERT INTO " + tableName.ToLower() + "(Date, Value) VALUES ('" + i.Date + "','" + i.Value + "')";
                MySqlCommand command = new MySqlCommand(queryString);
                command.Connection = connection;
                command.ExecuteNonQuery();
            }
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
    /// Get the list of items to be appended
    /// </summary>
    /// <param name="entityList">Entity list</param>
    /// <param name="tableName">Table mane</param>
    /// <returns>Entity list</returns>
    private List<DataEntity> GetUpdateList(List<DataEntity> entityList, string tableName)
    {
        DateTime dt = DateTime.MinValue;
        List<DataEntity> localList = new List<DataEntity>();
        

        MySqlConnection connection = new MySqlConnection(set.dataConnectionString);
        try
        {
            localList.AddRange(entityList);
            connection.Open();
            string queryString = "SELECT MAX(DATE) FROM " + tableName.ToLower();
            MySqlCommand countCommand = new MySqlCommand(queryString);
            countCommand.Connection = connection;
            object o = countCommand.ExecuteScalar();

            try
            {
                dt = Convert.ToDateTime(o);
            }
            catch (Exception)
            {
            	//do nothing
            }
            foreach (DataEntity s in entityList)
            {
                if (Convert.ToDateTime(s.Date) <= dt)
                {
                    localList.Remove(s);
                }
            }
        }
        catch (Exception ex)
        {
            throw ex;
        }
        finally
        {
            connection.Close();
        }

        if (localList.Count > 1)
        {
            try
            {
                DateTime d1 = Convert.ToDateTime(localList[0].Date);
                DateTime d2 = Convert.ToDateTime(localList[localList.Count - 1].Date);
                if (d1 >= d2)
                {
                    localList.Reverse();
                }
            }
            catch { }
        }

        return localList;
    }
}

