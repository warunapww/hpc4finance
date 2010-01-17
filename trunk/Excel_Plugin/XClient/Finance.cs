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
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Remoting;
using System.Diagnostics;
using System.IO;
using XClient.com.xignite.www;
using XClient.com.xignite;
using System.Configuration;
using XClient.Properties;
using System.Threading;
using System.Security;

namespace XClient
{
    public partial class Finance : UserControl
    {
        /// <summary>
        /// Finance Constructor
        /// </summary>
        public Finance()
        {
            InitializeComponent();

            updateButton.Enabled = true;
            filePathTextBox.Clear();
           
        }


        /// <summary>
        /// Event openFileDialog_FileOk
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void openFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            filePathTextBox.Text = openFileDialog.FileName;    
        }

        /// <summary>
        /// Event Finance_Load
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void Finance_Load(object sender, EventArgs e)
        {
            try
            {
                PopulateWithModels(this.modelComboBox);

            }
            catch { }

            try
            {
                PopulateWithPeriodTyps(this.periodTypeComboBox);
                PopulateWithPeriodTyps2(this.periodTypeComboBox2);
            }
            catch {}

            try
            {
                PopulateWithParaFormat(this.paraFormatTextBox, modelComboBox.SelectedItem.ToString());
            }
            catch { }

            try
            {
                PopulateWithTables(this.tablsComboBox);
                PopulateWithTables(this.tablesComboBox);
            }
            catch { }
            try
            {
                PopulateWithInterestRateTypes(this.rateTypesComboBox);
            }
            catch { }
            
        }

        /// <summary>
        /// Populate the ComboBox with time period types
        /// </summary>
        /// <param name="cb">ComboBox</param>
        private void PopulateWithPeriodTyps(ComboBox cb)
        {
            cb.Items.Clear();

            cb.Items.Add("Daily");
            cb.Items.Add("Monthly");
            cb.Items.Add("Quarterly");
            cb.Items.Add("Yearly");

            try
            {
                cb.SelectedIndex = 0;
            }
            catch (Exception)
            { }
        }
        private void PopulateWithPeriodTyps2(ComboBox cb)
        {
            cb.Items.Clear();

            cb.Items.Add("Days");
            cb.Items.Add("Months");
            cb.Items.Add("Quarters");
            cb.Items.Add("Years");

            try
            {
                cb.SelectedIndex = 0;
            }
            catch (Exception)
            { }
        }
        /// <summary>
        /// Populate the TextBox with parameter format for corresponding model
        /// </summary>
        /// <param name="tb">TextBox</param>
        /// <param name="model">model</param>
        private void PopulateWithParaFormat(TextBox tb, string model)
        {
            tb.Clear();
            tb.Text = new HPC_FinanceDBManager().GetParaList(model);
        }

        /// <summary>
        /// Populate the ComboBox with all models
        /// </summary>
        /// <param name="cb">ComboBox</param>
        private void PopulateWithModels(ComboBox cb)
        {
            HPC_FinanceDBManager manager = new HPC_FinanceDBManager();
            cb.Items.Clear();

            List<string> tablesList = manager.GetModels();
            foreach (string s in tablesList)
            {
                cb.Items.Add(s);
            }
            try
            {
                cb.SelectedIndex = 0;
            }
            catch (Exception)
            { }
        }

        /// <summary>
        /// Populate the ComboBox with all tbles
        /// </summary>
        /// <param name="cb">ComboBox</param>
        private void PopulateWithTables(ComboBox cb)
        {
            DataDBTableManager tManager = new DataDBTableManager();
            cb.Items.Clear();
            List<string> tablesList = tManager.GetAllTables();
            foreach (string s in tablesList)
            {
                cb.Items.Add(s.ToUpper());
            }
            try
            {
                cb.SelectedIndex = 0;
            }
            catch (Exception)
            { }

        }

        /// <summary>
        /// Event usingDataRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void usingDataRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (usingParaRadioButton.Checked)
            {
                panel2.Enabled = true;
                panel1.Enabled = false;
                groupBox3.Visible = true;
            }
            else
            {
                panel2.Enabled = false;
                panel1.Enabled = true;
                groupBox3.Visible = false;
            }
        }

        /// <summary>
        /// Event usingParaRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void usingParaRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (usingDataRadioButton.Checked)
            {
                panel1.Enabled = true;
                panel2.Enabled = false;
                groupBox3.Visible = false;
            }
            else
            {
                panel1.Enabled = false;
                panel2.Enabled = true;
                groupBox3.Visible = true;
            }
        }


        /// <summary>
        /// Event browseButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void browseButton_Click(object sender, EventArgs e)
        {
            openFileDialog.ShowDialog();
        }

        /// <summary>
        /// Event addButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void addButton_Click(object sender, EventArgs e)
        {
            if (tableNameTextBox.Text.Trim().Length > 0)
            {
                try
                {
                    new Thread(new ThreadStart(this.AddOperationWorker)).Start();
                }
                catch (ThreadStartException)
                { }
                catch (InvalidOperationException)
                { }
                catch (SecurityException)
                { }
                catch (OutOfMemoryException)
                { }
            }
            else
            {
                MessageBox.Show("Enter Table Name!");
            }
        }

        /// <summary>
        /// Worker of Add operation
        /// </summary>
        private void AddOperationWorker()
        {
            try
            {
                string table = tableNameTextBox.Text;
                tableNameTextBox.Clear();

                DataDBTableManager tManager = new DataDBTableManager();
                string[] array = table.Split(new Char[] { ' ' });
                string temp = null;
                foreach (string s in array)
                {
                    temp += s;
                }
                tManager.CreateTable(temp);
                PopulateWithTables(this.tablsComboBox);
                PopulateWithTables(this.tablesComboBox);
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid Operation or Database Connection!");
            }
        }

        /// <summary>
        /// Event updateButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void updateButton_Click(object sender, EventArgs e)
        {
            //UpdateOperationWorker();
            if (infileRadioButton.Checked)
            {
                if (!File.Exists(filePathTextBox.Text))
                {
                    MessageBox.Show("File Does Not Exist!");
                    return;
                }
            }

            try
            {
                new Thread(new ThreadStart(this.UpdateOperationWorker)).Start();
            }
            catch (ThreadStartException)
            { }
            catch (InvalidOperationException)
            { }
            catch (SecurityException)
            { }
            catch (OutOfMemoryException)
            { }

        }

        /// <summary>
        /// Worker method for update operation
        /// </summary>
        private void UpdateOperationWorker()
        {
            string path = filePathTextBox.Text;
            string table = tablsComboBox.SelectedItem.ToString();
            filePathTextBox.Clear();
            MessageBox.Show(table + " Update Started");

            try
            {
                if (infileRadioButton.Checked)
                {
                    new DataWriter().WriteData(new DataFetcher().GetDataEntityList(path), table);
                    filePathTextBox.Clear();
                }

                if (webServiceRadioButton.Checked)
                {
                    if (ratesRadioButton.Checked)
                    {
                        new WSIRWriter().WriteData(new WSIRFetcher().GetDataEntityList(rateTypesComboBox.SelectedItem.ToString(), fromDate.Value.ToShortDateString(), toDate.Value.ToShortDateString()), tablsComboBox.SelectedItem.ToString());
                    }
                    else
                    {
                        new WSSPWriter().WriteData(new WSSPFetcher().GetDataEntityList(identifier.Text, fromDate.Value.ToShortDateString(), toDate.Value.ToShortDateString()), tablsComboBox.SelectedItem.ToString());
                    }
                }
                MessageBox.Show(table + " Update Complete");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
           
        }

        /// <summary>
        /// Event deleteButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void deleteButton_Click(object sender, EventArgs e)
        {
            new Thread(new ThreadStart(this.DeleteOperationWorker)).Start();
        }

        /// <summary>
        /// Worker thread for delete operation
        /// </summary>
        private void DeleteOperationWorker()
        {
            if (tablsComboBox.Items.Count > 0)
            {
                try
                {
                    DataDBTableManager tManager = new DataDBTableManager();
                    tManager.DeleteTable(tablsComboBox.SelectedItem.ToString());
                    PopulateWithTables(this.tablsComboBox);
                    PopulateWithTables(this.tablesComboBox);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        /// <summary>
        /// Event infileRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void infileRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (infileRadioButton.Checked)
            {
                filePanel.Enabled = true;
                wsPanel.Enabled = false;
            }
        }

        /// <summary>
        /// Event webServiceRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void webServiceRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (webServiceRadioButton.Checked)
            {
                wsPanel.Enabled = true;
                filePanel.Enabled = false;
            }
        }

        /// <summary>
        /// Populates combo box with supporting interest rate types
        /// </summary>
        /// <param name="comboBox">Combobox</param>
        private void PopulateWithInterestRateTypes(ComboBox cb)
        {
            cb.Items.Clear();
            Type rates = typeof(RateTypes);
            string[] typesArray = Enum.GetNames(rates);
            foreach (string s in typesArray)
            {
                cb.Items.Add(s);
            }
            try
            {
                cb.SelectedIndex = 0;
            }
            catch (Exception)
            { }
        }




        /// <summary>
        /// Worker for execute operation
        /// </summary>
        private void ExecuteWorker()
        {
            try
            {


                int per = int.Parse(periodTextBox.Text);
                //string period = Convert.ToString(per * int.Parse(periodTypeComboBox2.SelectedItem.ToString()));
                this.periodTextBox.Clear();
                string model = modelComboBox.SelectedItem.ToString();
                string fullArg = null;
                string path = new HPC_FinanceDBManager().GetPath(model);
                string paraList = null;
                string result = null;
                Settings set = Settings.Default;
                string table = tablesComboBox.SelectedItem.ToString();
                string periodType = periodTypeComboBox.SelectedItem.ToString();
                string periodType2 = periodTypeComboBox2.SelectedItem.ToString();
                double factor1, factor2;
                if (periodType.Equals("Daily"))
                {
                    factor1 = 252;
                }
                else if (periodType.Equals("Monthly"))
                {
                    factor1 = 12;
                }
                else if (periodType.Equals("Quarterly"))
                {
                    factor1 = 4;
                }
                else
                {
                    factor1 = 1;
                }
                if (periodType2.Equals("Days"))
                {
                    factor2 = 252;
                }
                else if (periodType2.Equals("Months"))
                {
                    factor2 = 12;
                }
                else if (periodType2.Equals("Quarters"))
                {
                    factor2 = 4;
                }
                else
                {
                    factor2 = 1;
                }
                int temp = (int)(per * (factor1 / factor2));
                string period = Convert.ToString(temp);
                string para = paraTextBox.Text;
                bool paraRadButtonStatus = usingParaRadioButton.Checked;
                bool dataRadButtonStatus = usingDataRadioButton.Checked;
                paraTextBox.Clear();
                string ticket = new ClientEngine().GetTicket();
                string dataS = "Manual";
                if (dataRadButtonStatus)
                {
                    dataS = table;
                }
                ListViewItem item = new ListViewItem(new string[] { ticket, model, periodType, period, set.nMC, set.nodes, dataS });
                lock (listView1)
                {
                    listView1.Items.Add(item);
                }


                MessageBox.Show("Model : " + model + "\n" + "Job Submitted" + "\n");


                if (paraRadButtonStatus)
                {
                    paraList = para;
                }
                if (dataRadButtonStatus)
                {
                    paraList = "-t" + " " + table.ToLower();
                }

                if (periodType.Equals("Daily"))
                {
                    paraList += " " + "-d" + " " + period;
                }
                else if (periodType.Equals("Monthly"))
                {
                    paraList += " " + "-m" + " " + period;
                }
                else if (periodType.Equals("Quarterly"))
                {
                    paraList += " " + "-q" + " " + period;
                }
                else
                {
                    paraList += " " + "-y" + " " + period;
                }

                paraList += " " + "-s" + " " + set.nMC;

                fullArg = set.nodes + " " + "-a" + " " + ticket + " " + path + " " + paraList;


                result = new ClientEngine().ExecuteJob(fullArg);
                try
                {
                    lock (listView1)
                    {
                        listView1.Items.Remove(item);
                    }

                }
                catch(Exception)
                { }

                string[] rCategiries = result.Split(new Char[] { ':' });


                if (rCategiries.Length == 1)
                {
                    if (rCategiries[0] == "")
                    {
                        MessageBox.Show("Contact Cluster Administrator");
                    }
                    else
                    {
                        string[] error = rCategiries[0].Split(new Char[] { ',' });
                        try
                        {
                            MessageBox.Show(error[1]);
                        }
                        catch (System.Exception)
                        {
                            MessageBox.Show("Contact Cluster Administrator");
                        }

                    }

                }
                else if (rCategiries.Length == 2)
                {

                    string title = "Model:" + " " + model + "," + periodType + ":" + " " + period + "," + "Simulations:" + " " + set.nMC + "," + "Nodes:" + " " + set.nodes;
                    string data = "Manual";
                    if (dataRadButtonStatus)
                    {
                        data = table;
                    }
                    title += "," + "Data:" + " " + data;
                    DrawToSpreadSheet(result, title);
                    
                }
                else
                {

                    MessageBox.Show("Contact Cluster Administrator");
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Contact Cluster Administrator");
            }

        }

        /// <summary>
        /// Validate parameters entered
        /// </summary>
        /// <returns>true/flase</returns>
        private bool ValidateParameters()
        {
            try
            {
                string[] current = paraTextBox.Text.Split(new Char[] { ' ' });
                string[] format = new HPC_FinanceDBManager().GetParaTypes(modelComboBox.SelectedItem.ToString()).Split(new Char[] { ',' });
                if(format.Length == 0)
                {
                    return true;
                }
                if(format.Length != current.Length)
                {
                    return false;
                }
                
                for(int i = 0; i < format.Length ; i++)
                {
                    if(format[i].Equals("double"))
                    {
                        double d = Convert.ToDouble(current[i]);
                    }
                    if(format[i].Equals("int"))
                    {
                        int ii = Convert.ToInt32(current[i]);
                    }
                }

                double temp = Convert.ToDouble(periodTextBox.Text);
            }
            catch (System.Exception)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Draw the final result in a spreadsheet
        /// </summary>
        /// <param name="result">result returned from server</param>
        /// <param name="title">meta data</param>
        private void DrawToSpreadSheet(string result, string title)
        {
            string[] results = result.Split(new Char[] { ':' });

            Microsoft.Office.Interop.Excel.Worksheet ws;

            if (Globals.ThisAddIn.Application.Workbooks.Count == 0)
            {
                Microsoft.Office.Interop.Excel.Workbook wb = Globals.ThisAddIn.Application.Workbooks.Add(Microsoft.Office.Interop.Excel.XlWBATemplate.xlWBATWorksheet);
                ws = (Microsoft.Office.Interop.Excel.Worksheet)wb.ActiveSheet;
            }
            else
            {
                ws = (Microsoft.Office.Interop.Excel.Worksheet)Globals.ThisAddIn.Application.ActiveWorkbook.Worksheets.Add(Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSheetType.xlWorksheet);
            }

            ws.Cells[1, 1] = "Time";
            ws.Cells[1, 2] = "Value";

            string[] titles = title.Split(new Char[] { ',' });
            if (titles.Length == 5)
            {
                ws.Cells[1, 4] = titles[0];
                ws.Cells[1, 7] = titles[1];
                ws.Cells[1, 10] = titles[2];
                ws.Cells[2, 4] = titles[3];
                ws.Cells[2, 7] = titles[4];
            }

            string[] values = results[1].Split(new Char[] { ',' });

            int paramCell = 0;
            double xUnit = 0;
            if (titles.Length == 5)
            {
                string[] periodArray = titles[1].Split(new Char[] { ':' });
                if (periodArray.Length == 2)
                {
                    try
                    {
                        double d = Convert.ToDouble(periodArray[1]);
                        xUnit = d / (values.Length - 1);
                    }
                    catch { }
                }
            }

            string periodType = "";
            if (titles.Length == 5)
            {
                string[] periodTypeArray = titles[1].Split(new Char[] { ':' });
                if (periodTypeArray.Length == 2)
                {
                    periodType = periodTypeArray[0];
                }
            }


            for (int i = 1; i < values.Length; i++)
            {
                double d = i * xUnit;
                ws.Cells[i + 1, 1] = d.ToString();
                ws.Cells[i + 1, 2] = values[i];

                paramCell = i + 4;
            }

            string[] paras = results[0].Split(new Char[] { ',' });
            for (int i = 1; i < paras.Length; i++)
            {
                string[] temp = paras[i].Split(new Char[] { '=' });
                ws.Cells[paramCell, 1] = temp[0];
                ws.Cells[paramCell, 2] = temp[1];
                paramCell++;
            }

            paramCell = 0;

            if (values.Length > 2)
            {


                Microsoft.Office.Interop.Excel.ChartObjects charts = (Microsoft.Office.Interop.Excel.ChartObjects)ws.ChartObjects(Type.Missing);
                Microsoft.Office.Interop.Excel.ChartObject chart = charts.Add(110, 40, 600, 300);
                Microsoft.Office.Interop.Excel.Chart xlChart = chart.Chart;
                Microsoft.Office.Interop.Excel.Range rg;
                int end = values.Length + 1;
                rg = ws.get_Range("A2", "B" + end.ToString());


                xlChart.Legend.Clear();

                Microsoft.Office.Interop.Excel.Axis x;
                x = (Microsoft.Office.Interop.Excel.Axis)xlChart.Axes(Microsoft.Office.Interop.Excel.XlAxisType.xlCategory, Microsoft.Office.Interop.Excel.XlAxisGroup.xlPrimary);
                x.HasTitle = true;
                x.AxisTitle.Text = "Time" + "(" + periodType + ")";

                Microsoft.Office.Interop.Excel.Axis y;
                y = (Microsoft.Office.Interop.Excel.Axis)xlChart.Axes(Microsoft.Office.Interop.Excel.XlAxisType.xlValue, Microsoft.Office.Interop.Excel.XlAxisGroup.xlPrimary);
                y.HasTitle = true;
                y.AxisTitle.Text = "Estimated Value";

                xlChart.ChartType = Microsoft.Office.Interop.Excel.XlChartType.xlXYScatterLinesNoMarkers;
                xlChart.SetSourceData(rg, Type.Missing);

                Microsoft.Office.Interop.Excel.Series s = (Microsoft.Office.Interop.Excel.Series)xlChart.SeriesCollection(1);
                s.Format.Line.Weight = (float)0.25;
                s.MarkerStyle = Microsoft.Office.Interop.Excel.XlMarkerStyle.xlMarkerStyleNone;
            }
        }

        /// <summary>
        /// Event modelComboBox_SelectedIndexChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void modelComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            PopulateWithParaFormat(this.paraFormatTextBox, this.modelComboBox.SelectedItem.ToString());
        }

        /// <summary>
        /// Event executeButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void executeButton_Click(object sender, EventArgs e)
        {

            if (usingParaRadioButton.Checked)
            {
                if(!ValidateParameters())
                {
                    MessageBox.Show("Parameters Invalid!");
                    return;
                }
                
            }

            if(!ValidatePeriods())
            {
                MessageBox.Show("Time Step Not Compatible!");
                return;
            }


            try
            {
                double d = Convert.ToDouble(periodTextBox.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("Period Invalid!");
                periodTextBox.Clear();
                return;
            }

            //ExecuteWorker();
            try
            {
                new Thread(new ThreadStart(this.ExecuteWorker)).Start();
            }
            catch (ThreadStartException)
            { }
            catch (InvalidOperationException)
            { }
            catch (SecurityException)
            { }
            catch (OutOfMemoryException)
            { }
        }

        /// <summary>
        /// Validating time step compatibility
        /// </summary>
        /// <returns>bool</returns>
        private bool ValidatePeriods()
        {
            try
            {
                int per = int.Parse(periodTextBox.Text);
                string periodType = periodTypeComboBox.SelectedItem.ToString();
                string periodType2 = periodTypeComboBox2.SelectedItem.ToString();
                double factor1, factor2;
                if (periodType.Equals("Daily"))
                {
                    factor1 = 252;
                }
                else if (periodType.Equals("Monthly"))
                {
                    factor1 = 12;
                }
                else if (periodType.Equals("Quarterly"))
                {
                    factor1 = 4;
                }
                else
                {
                    factor1 = 1;
                }
                if (periodType2.Equals("Days"))
                {
                    factor2 = 252;
                }
                else if (periodType2.Equals("Months"))
                {
                    factor2 = 12;
                }
                else if (periodType2.Equals("Quarters"))
                {
                    factor2 = 4;
                }
                else
                {
                    factor2 = 1;
                }

                if (factor1 * per / factor2 < 1)
                {
                    return false;
                }
                return true;
            }
            catch (System.Exception)
            {
                return false;
            }
            
        }

        /// <summary>
        /// Event ratesRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void ratesRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (ratesRadioButton.Checked)
            {
                rateTypesComboBox.Enabled = true;
                identifier.Enabled = false;
            }
        }

        /// <summary>
        /// Event quotesRadioButton_CheckedChanged
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void quotesRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (quotesRadioButton.Checked)
            {
                rateTypesComboBox.Enabled = false;
                identifier.Enabled = true;
            }
        }

        /// <summary>
        /// Event endJobButton_Click
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void endJobButton_Click(object sender, EventArgs e)
        {
            try
            {
                new Thread(new ThreadStart(this.KillJobWorker)).Start();
            }
            catch (ThreadStartException)
            { }
            catch (InvalidOperationException)
            { }
            catch (SecurityException)
            { }
            catch (OutOfMemoryException)
            { }
        }

        /// <summary>
        /// Worker procedure for thread kill operation
        /// </summary>
        private void KillJobWorker()
        {
            try
            {
                string key;
                lock (listView1)
                {
                    key = listView1.SelectedItems[0].SubItems[0].Text;
                    listView1.SelectedItems[0].Remove();
                }
                string args = "mpdkilljob -a " + key;
                new ClientEngine().KillJob(args);
            }
            catch (Exception)
            { }
        }

    }
}
