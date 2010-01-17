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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Configuration;
using XClient.Properties;

namespace XClient
{
    public partial class ServerSetingsForm : Form
    {
        /// <summary>
        /// COnstructor
        /// </summary>
        public ServerSetingsForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Event ServerSetingsForm_Load
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void ServerSetingsForm_Load(object sender, EventArgs e)
        {
            try
            {
                Settings set = Settings.Default;
                textBox1.Text = set.mnIP;
                textBox2.Text = set.mnServicePort; 
            }
            catch (Exception)
            {
            } 
        }

        /// <summary>
        /// Event button1_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                Settings set = Settings.Default;
                set.mnIP = textBox1.Text;
                set.mnServicePort = Convert.ToInt32(textBox2.Text).ToString();
                set.Save();

                this.Close();
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid Port Number!");
            }
        }

    }
}
