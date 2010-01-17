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
    public partial class DatabaseSettingsForm : Form
    {
        public DatabaseSettingsForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Event Form_Load
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">Event argument</param>
        private void DatabaseSettingsForm_Load(object sender, EventArgs e)
        {
            try
            {
                Settings set = Settings.Default;
                textBox1.Text = set.mnConnectionString;
                textBox2.Text = set.dataConnectionString;
            }
            catch (Exception)
            {
            }        
        }

        /// <summary>
        /// Event button1_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event argument</param>
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                Settings set = Settings.Default;
                set.mnConnectionString = textBox1.Text;
                set.dataConnectionString = textBox2.Text;
                set.Save();

                this.Close();
            }
            catch (Exception)
            {
            }
        }

       
    }
}