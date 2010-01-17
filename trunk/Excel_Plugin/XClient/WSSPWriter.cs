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
using System.Configuration;
using MySql.Data.MySqlClient;
using XClient.com.xignite.www;
using XClient.Properties;

class WSSPWriter
{

    /// <summary>
    /// Write date to the database when source is a webservice
    /// </summary>
    /// <param name="quotes">HistoricalQuotes</param>
    /// <param name="tableName">Name of the table to insert data</param>
    public void WriteData(HistoricalQuotes quotes, string tableName)
    {
        try
        {
            List<DataEntity> entityList = new List<DataEntity>();

            foreach (HistoricalQuote q in quotes.Quotes)
            {
                DataEntity s = new DataEntity();
                s.Date = UStoBrithishDateFormat(q.Date);
                s.Value = q.LastClose.ToString();
                entityList.Add(s);
            }

            new DataWriter().WriteData(entityList, tableName);
        }
        catch (Exception ex)
        {
            new Exception("Web Service Credentials Invalid!");
        }
    }

    /// <summary>
    /// Convert date from US fromat to UK format
    /// </summary>
    /// <param name="p">Date in US format</param>
    /// <returns>Date in UK format</returns>
    private string UStoBrithishDateFormat(string p)
    {
        string[] array = p.Split(new Char[] { '/' });
        string temp = array[2] + "-" + array[0] + "-" + array[1];
        return temp.Trim();
    }


}

