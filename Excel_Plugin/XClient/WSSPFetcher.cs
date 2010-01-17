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
using XClient.com.xignite.www;
using System.Configuration;
using XClient.Properties;

class WSSPFetcher 
{
    Settings set = Settings.Default;

    /// <summary>
    /// Get historical quotes from Xignite web service
    /// </summary>
    /// <param name="identifier">Company identifier</param>
    /// <param name="startdate">Start date [dd/mm/yyyy]</param>
    /// <param name="enddate">End date [dd/mm/yyyy]</param>
    /// <returns>HistoricalQuotes</returns>
    public HistoricalQuotes GetDataEntityList(string identifier, string startdate, string enddate)
    {
        XigniteHistorical historicalService = new XigniteHistorical();
        Header header = new Header();
        header.Username = set.username;
        header.Password = set.password;

        historicalService.HeaderValue = header;
        HistoricalQuotes historicalQuotes;
        try
        {
            
            historicalQuotes = historicalService.GetHistoricalQuotesRange(identifier, IdentifierTypes.Symbol, startdate, enddate);
            //if (historicalQuotes == null)
            //{
            //    /// add error processing here 
            //    /// this condition could be caused by an HTTP error (404,500...)
            //    Console.Write("Service is unavailable at this time.");
            //}
            //else
            //{
            //    switch (historicalQuotes.Outcome)
            //    {
            //        case OutcomeTypes.RegistrationError:
            //            /// add processing for handling subscription problems, e.g.
            //            Console.Write("Our subscription to this service has expired.");
            //            break;
            //        case OutcomeTypes.RequestError:
            //            /// add processing for hserviceandling request problems, e.g. 
            //            /// you could pass back the info message received from the 
            //            Console.Write(historicalQuotes.Message);
            //            break;
            //        case OutcomeTypes.SystemError:
            //            /// add processing for handling system problems, e.g. 
            //            Console.Write("Service is unavailable at this time.");
            //            break;
            //        default:
            //            /// add processing for displaying the results, e.g. 
            //            /// since the return class contains an array
            //            /// we just display its count 
            //            /// each instance and its values can be easily accessed
            //            //Console.Write(objHistoricalQuotes.Length);
            //            break;
            //    }
            //}
        }
        catch (Exception ex)
        {
            throw new Exception("Connection to Web Service failed!");
        }
        return historicalQuotes;
    }
}

