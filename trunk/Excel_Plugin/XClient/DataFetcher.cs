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
using System.IO;


class DataFetcher
{
    /// <summary>
    /// Get data entity list from .csv file
    /// </summary>
    /// <param name="dataFile">.CSV file</param>
    /// <returns>DataEntity list</returns>
    public List<DataEntity> GetDataEntityList(string dataFile)
    {
        List<DataEntity> entityList = new List<DataEntity>();
        try
        {
            using (StreamReader sr = new StreamReader(dataFile))
            {
                string line;
                while ((line = sr.ReadLine()) != null)
                {
                    string[] split = line.Split(new string[] { ",", ",,", ",,,", ",,,," }, StringSplitOptions.RemoveEmptyEntries);
                    DataEntity entity = new DataEntity();
                    try
                    {
                        DateTime dt = Convert.ToDateTime(split[0]); //Date parser

                        entity.Date = dt.Year.ToString() + "-" + dt.Month.ToString() + "-" + dt.Day.ToString();
                        entity.Value = split[1];
                        entityList.Add(entity);
                    }
                    catch (Exception)
                    {
                    }
                }
            }
        }
        catch (Exception ex)
        {
            throw ex;
        }
        return entityList;
    }
}

