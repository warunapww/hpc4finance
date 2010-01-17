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
using System.Net.Sockets;
using System.Configuration;
using XClient.Properties;


class ClientEngine
{
   /// <summary>
   /// Execute a job in the cluster
   /// </summary>
   /// <param name="args">Execution string to be passed to the server</param>
   /// <returns>String of results</returns>
    public string ExecuteJob(string args)
    {
        String responseData = String.Empty;
        try
        {
            Settings set = Settings.Default;
            string mnIp = set.mnIP;
            int port = Convert.ToInt32(set.mnServicePort);

            TcpClient client = new TcpClient(mnIp, port);
            Byte[] data = System.Text.Encoding.ASCII.GetBytes(args);

            NetworkStream stream = client.GetStream();
            stream.Write(data, 0, data.Length);
            data = new Byte[1];

            do
            {
                Int32 bytes = stream.Read(data, 0, data.Length);
                responseData += System.Text.Encoding.ASCII.GetString(data, 0, bytes);
            }
            while (stream.DataAvailable);

            stream.Close();
            client.Close();
        }
        catch (Exception)
        {
        }
        return responseData;
    }

    /// <summary>
    /// Get a job ID from the server
    /// </summary>
    /// <returns>ticket</returns>
    public string GetTicket()
    {
        string ticket = String.Empty;
        string args = "ticket";
        try
        {
            Settings set = Settings.Default;
            string mnIp = set.mnIP;
            int port = Convert.ToInt32(set.mnServicePort);

            TcpClient client = new TcpClient(mnIp, port);
            Byte[] data = System.Text.Encoding.ASCII.GetBytes(args);

            NetworkStream stream = client.GetStream();
            stream.Write(data, 0, data.Length);
            data = new Byte[1];

            do
            {
                Int32 bytes = stream.Read(data, 0, data.Length);
                ticket += System.Text.Encoding.ASCII.GetString(data, 0, bytes);
            }
            while (stream.DataAvailable);

            stream.Close();
            client.Close();
        }
        catch (Exception)
        {
        }
        return ticket;
    }

    /// <summary>
    /// Request to kill a Job
    /// </summary>
    /// <param name="args">request string</param>
    public void KillJob(string args)
    {
        string ticket = String.Empty;
        try
        {
            Settings set = Settings.Default;
            string mnIp = set.mnIP;
            int port = Convert.ToInt32(set.mnServicePort);

            TcpClient client = new TcpClient(mnIp, port);
            Byte[] data = System.Text.Encoding.ASCII.GetBytes(args);

            NetworkStream stream = client.GetStream();
            stream.Write(data, 0, data.Length);

            stream.Close();
            client.Close();
        }
        catch (Exception)
        {
        }
    }
}

