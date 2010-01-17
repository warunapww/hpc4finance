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
using XClient.com.xignite;
using XClient.Properties;

class WSIRWriter
{


    /// <summary>
    /// Write interest rate data in to a given data table
    /// </summary>
    /// <param name="interestRates">List of interest rates</param>
    /// <param name="tableName">Table name</param>
    public void WriteData(HistoricalInterestRates interestRates, string tableName)
    {
        try
        {
            List<DataEntity> entityList = new List<DataEntity>();

            foreach (Rate r in interestRates.Rates)
            {
                DataEntity i = new DataEntity();
                i.Date = UStoBrithishDateFormat(r.Date);
                i.Value = r.Value.ToString();
                entityList.Add(i);
            }

            new DataWriter().WriteData(entityList, tableName);
        }
        catch (Exception ex)
        {
            throw new Exception("Web Service Credentials Invalid!");
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

