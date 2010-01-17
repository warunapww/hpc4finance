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

namespace XClient
{
    partial class Finance
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.tabWindow = new System.Windows.Forms.TabControl();
            this.tabForecast = new System.Windows.Forms.TabPage();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.periodTypeComboBox2 = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.periodTextBox = new System.Windows.Forms.TextBox();
            this.periodTypeComboBox = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.executeButton = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.paraFormatTextBox = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.usingParaRadioButton = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.tablesComboBox = new System.Windows.Forms.ComboBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.paraTextBox = new System.Windows.Forms.TextBox();
            this.usingDataRadioButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.modelComboBox = new System.Windows.Forms.ComboBox();
            this.tabData = new System.Windows.Forms.TabPage();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.tableNameTextBox = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.addButton = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.wsPanel = new System.Windows.Forms.Panel();
            this.identifier = new System.Windows.Forms.TextBox();
            this.quotesRadioButton = new System.Windows.Forms.RadioButton();
            this.ratesRadioButton = new System.Windows.Forms.RadioButton();
            this.rateTypesComboBox = new System.Windows.Forms.ComboBox();
            this.toDate = new System.Windows.Forms.DateTimePicker();
            this.fromDate = new System.Windows.Forms.DateTimePicker();
            this.label15 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.webServiceRadioButton = new System.Windows.Forms.RadioButton();
            this.infileRadioButton = new System.Windows.Forms.RadioButton();
            this.filePanel = new System.Windows.Forms.Panel();
            this.label20 = new System.Windows.Forms.Label();
            this.irBrowseButton = new System.Windows.Forms.Button();
            this.filePathTextBox = new System.Windows.Forms.TextBox();
            this.updateButton = new System.Windows.Forms.Button();
            this.deleteButton = new System.Windows.Forms.Button();
            this.tablsComboBox = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.endJobButton = new System.Windows.Forms.Button();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.tabWindow.SuspendLayout();
            this.tabForecast.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabData.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.wsPanel.SuspendLayout();
            this.filePanel.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog
            // 
            this.openFileDialog.DefaultExt = "csv";
            this.openFileDialog.Filter = "CSV files | *.csv";
            this.openFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog_FileOk);
            // 
            // tabWindow
            // 
            this.tabWindow.Controls.Add(this.tabForecast);
            this.tabWindow.Controls.Add(this.tabData);
            this.tabWindow.Controls.Add(this.tabPage1);
            this.tabWindow.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabWindow.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.tabWindow.Location = new System.Drawing.Point(0, 0);
            this.tabWindow.Name = "tabWindow";
            this.tabWindow.SelectedIndex = 0;
            this.tabWindow.Size = new System.Drawing.Size(175, 530);
            this.tabWindow.TabIndex = 32;
            // 
            // tabForecast
            // 
            this.tabForecast.BackColor = System.Drawing.Color.AliceBlue;
            this.tabForecast.Controls.Add(this.groupBox8);
            this.tabForecast.Controls.Add(this.executeButton);
            this.tabForecast.Controls.Add(this.groupBox3);
            this.tabForecast.Controls.Add(this.groupBox2);
            this.tabForecast.Controls.Add(this.groupBox1);
            this.tabForecast.ForeColor = System.Drawing.Color.DarkBlue;
            this.tabForecast.Location = new System.Drawing.Point(4, 22);
            this.tabForecast.Name = "tabForecast";
            this.tabForecast.Padding = new System.Windows.Forms.Padding(3);
            this.tabForecast.Size = new System.Drawing.Size(167, 504);
            this.tabForecast.TabIndex = 0;
            this.tabForecast.Text = "Forecast";
            this.tabForecast.UseVisualStyleBackColor = true;
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.periodTypeComboBox2);
            this.groupBox8.Controls.Add(this.label10);
            this.groupBox8.Controls.Add(this.periodTextBox);
            this.groupBox8.Controls.Add(this.periodTypeComboBox);
            this.groupBox8.Controls.Add(this.label5);
            this.groupBox8.Location = new System.Drawing.Point(-1, 281);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(168, 75);
            this.groupBox8.TabIndex = 37;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Forecasts";
            // 
            // periodTypeComboBox2
            // 
            this.periodTypeComboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.periodTypeComboBox2.FormattingEnabled = true;
            this.periodTypeComboBox2.Items.AddRange(new object[] {
            "daily",
            "monthly",
            "quarterly",
            "yearly"});
            this.periodTypeComboBox2.Location = new System.Drawing.Point(85, 41);
            this.periodTypeComboBox2.Name = "periodTypeComboBox2";
            this.periodTypeComboBox2.Size = new System.Drawing.Size(75, 21);
            this.periodTypeComboBox2.TabIndex = 29;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(4, 45);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(42, 13);
            this.label10.TabIndex = 28;
            this.label10.Text = "Periods";
            // 
            // periodTextBox
            // 
            this.periodTextBox.Location = new System.Drawing.Point(47, 42);
            this.periodTextBox.Name = "periodTextBox";
            this.periodTextBox.Size = new System.Drawing.Size(34, 20);
            this.periodTextBox.TabIndex = 27;
            this.periodTextBox.TabStop = false;
            // 
            // periodTypeComboBox
            // 
            this.periodTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.periodTypeComboBox.FormattingEnabled = true;
            this.periodTypeComboBox.Location = new System.Drawing.Point(85, 15);
            this.periodTypeComboBox.Name = "periodTypeComboBox";
            this.periodTypeComboBox.Size = new System.Drawing.Size(75, 21);
            this.periodTypeComboBox.TabIndex = 26;
            this.periodTypeComboBox.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(4, 18);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(55, 13);
            this.label5.TabIndex = 25;
            this.label5.Text = "Time Step";
            // 
            // executeButton
            // 
            this.executeButton.Location = new System.Drawing.Point(84, 369);
            this.executeButton.Name = "executeButton";
            this.executeButton.Size = new System.Drawing.Size(75, 23);
            this.executeButton.TabIndex = 36;
            this.executeButton.TabStop = false;
            this.executeButton.Text = "Execute";
            this.executeButton.UseVisualStyleBackColor = true;
            this.executeButton.Click += new System.EventHandler(this.executeButton_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.paraFormatTextBox);
            this.groupBox3.Location = new System.Drawing.Point(3, 405);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(168, 93);
            this.groupBox3.TabIndex = 34;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Parameter Format";
            // 
            // paraFormatTextBox
            // 
            this.paraFormatTextBox.Location = new System.Drawing.Point(9, 19);
            this.paraFormatTextBox.Multiline = true;
            this.paraFormatTextBox.Name = "paraFormatTextBox";
            this.paraFormatTextBox.ReadOnly = true;
            this.paraFormatTextBox.Size = new System.Drawing.Size(150, 55);
            this.paraFormatTextBox.TabIndex = 100;
            this.paraFormatTextBox.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.usingParaRadioButton);
            this.groupBox2.Controls.Add(this.panel1);
            this.groupBox2.Controls.Add(this.panel2);
            this.groupBox2.Controls.Add(this.usingDataRadioButton);
            this.groupBox2.Location = new System.Drawing.Point(0, 94);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(168, 178);
            this.groupBox2.TabIndex = 33;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Parameters";
            // 
            // usingParaRadioButton
            // 
            this.usingParaRadioButton.AutoSize = true;
            this.usingParaRadioButton.BackColor = System.Drawing.Color.Transparent;
            this.usingParaRadioButton.Checked = true;
            this.usingParaRadioButton.Location = new System.Drawing.Point(5, 13);
            this.usingParaRadioButton.Name = "usingParaRadioButton";
            this.usingParaRadioButton.Size = new System.Drawing.Size(108, 17);
            this.usingParaRadioButton.TabIndex = 2;
            this.usingParaRadioButton.TabStop = true;
            this.usingParaRadioButton.Text = "Using Parameters";
            this.usingParaRadioButton.UseVisualStyleBackColor = false;
            this.usingParaRadioButton.CheckedChanged += new System.EventHandler(this.usingParaRadioButton_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Transparent;
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.tablesComboBox);
            this.panel1.Enabled = false;
            this.panel1.Location = new System.Drawing.Point(20, 107);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(142, 59);
            this.panel1.TabIndex = 33;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(100, 13);
            this.label3.TabIndex = 24;
            this.label3.Text = "Select Data Source";
            // 
            // tablesComboBox
            // 
            this.tablesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tablesComboBox.FormattingEnabled = true;
            this.tablesComboBox.Location = new System.Drawing.Point(7, 29);
            this.tablesComboBox.Name = "tablesComboBox";
            this.tablesComboBox.Size = new System.Drawing.Size(132, 21);
            this.tablesComboBox.TabIndex = 5;
            this.tablesComboBox.TabStop = false;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Transparent;
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.paraTextBox);
            this.panel2.Location = new System.Drawing.Point(20, 36);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(142, 43);
            this.panel2.TabIndex = 32;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(125, 13);
            this.label2.TabIndex = 23;
            this.label2.Text = "Space Seperated Values";
            // 
            // paraTextBox
            // 
            this.paraTextBox.Location = new System.Drawing.Point(7, 19);
            this.paraTextBox.Name = "paraTextBox";
            this.paraTextBox.Size = new System.Drawing.Size(132, 20);
            this.paraTextBox.TabIndex = 3;
            this.paraTextBox.TabStop = false;
            // 
            // usingDataRadioButton
            // 
            this.usingDataRadioButton.AutoSize = true;
            this.usingDataRadioButton.BackColor = System.Drawing.Color.Transparent;
            this.usingDataRadioButton.Location = new System.Drawing.Point(5, 86);
            this.usingDataRadioButton.Name = "usingDataRadioButton";
            this.usingDataRadioButton.Size = new System.Drawing.Size(124, 17);
            this.usingDataRadioButton.TabIndex = 4;
            this.usingDataRadioButton.Text = "Using Historical Data";
            this.usingDataRadioButton.UseVisualStyleBackColor = false;
            this.usingDataRadioButton.CheckedChanged += new System.EventHandler(this.usingDataRadioButton_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.modelComboBox);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(165, 82);
            this.groupBox1.TabIndex = 32;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Model ";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Location = new System.Drawing.Point(6, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 30;
            this.label1.Text = "Select Model";
            // 
            // modelComboBox
            // 
            this.modelComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modelComboBox.FormattingEnabled = true;
            this.modelComboBox.Location = new System.Drawing.Point(9, 43);
            this.modelComboBox.Name = "modelComboBox";
            this.modelComboBox.Size = new System.Drawing.Size(150, 21);
            this.modelComboBox.TabIndex = 1;
            this.modelComboBox.TabStop = false;
            this.modelComboBox.SelectedIndexChanged += new System.EventHandler(this.modelComboBox_SelectedIndexChanged);
            // 
            // tabData
            // 
            this.tabData.BackColor = System.Drawing.Color.AliceBlue;
            this.tabData.Controls.Add(this.groupBox7);
            this.tabData.Controls.Add(this.groupBox6);
            this.tabData.ForeColor = System.Drawing.Color.DarkBlue;
            this.tabData.Location = new System.Drawing.Point(4, 22);
            this.tabData.Name = "tabData";
            this.tabData.Padding = new System.Windows.Forms.Padding(3);
            this.tabData.Size = new System.Drawing.Size(167, 504);
            this.tabData.TabIndex = 2;
            this.tabData.Text = "Data";
            this.tabData.UseVisualStyleBackColor = true;
            // 
            // groupBox7
            // 
            this.groupBox7.BackColor = System.Drawing.Color.AliceBlue;
            this.groupBox7.Controls.Add(this.tableNameTextBox);
            this.groupBox7.Controls.Add(this.label12);
            this.groupBox7.Controls.Add(this.addButton);
            this.groupBox7.Location = new System.Drawing.Point(1, 433);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(165, 66);
            this.groupBox7.TabIndex = 36;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Add Table";
            // 
            // tableNameTextBox
            // 
            this.tableNameTextBox.Location = new System.Drawing.Point(6, 34);
            this.tableNameTextBox.Name = "tableNameTextBox";
            this.tableNameTextBox.Size = new System.Drawing.Size(71, 20);
            this.tableNameTextBox.TabIndex = 12;
            this.tableNameTextBox.TabStop = false;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.ForeColor = System.Drawing.Color.DarkBlue;
            this.label12.Location = new System.Drawing.Point(6, 18);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(65, 13);
            this.label12.TabIndex = 33;
            this.label12.Text = "Table Name";
            // 
            // addButton
            // 
            this.addButton.Location = new System.Drawing.Point(84, 32);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(75, 23);
            this.addButton.TabIndex = 13;
            this.addButton.TabStop = false;
            this.addButton.Text = "Add";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.BackColor = System.Drawing.Color.AliceBlue;
            this.groupBox6.Controls.Add(this.wsPanel);
            this.groupBox6.Controls.Add(this.webServiceRadioButton);
            this.groupBox6.Controls.Add(this.infileRadioButton);
            this.groupBox6.Controls.Add(this.filePanel);
            this.groupBox6.Controls.Add(this.updateButton);
            this.groupBox6.Controls.Add(this.deleteButton);
            this.groupBox6.Controls.Add(this.tablsComboBox);
            this.groupBox6.Controls.Add(this.label11);
            this.groupBox6.Location = new System.Drawing.Point(2, 3);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(165, 424);
            this.groupBox6.TabIndex = 35;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Update Database";
            // 
            // wsPanel
            // 
            this.wsPanel.Controls.Add(this.identifier);
            this.wsPanel.Controls.Add(this.quotesRadioButton);
            this.wsPanel.Controls.Add(this.ratesRadioButton);
            this.wsPanel.Controls.Add(this.rateTypesComboBox);
            this.wsPanel.Controls.Add(this.toDate);
            this.wsPanel.Controls.Add(this.fromDate);
            this.wsPanel.Controls.Add(this.label15);
            this.wsPanel.Controls.Add(this.label18);
            this.wsPanel.Enabled = false;
            this.wsPanel.Location = new System.Drawing.Point(19, 202);
            this.wsPanel.Name = "wsPanel";
            this.wsPanel.Size = new System.Drawing.Size(141, 182);
            this.wsPanel.TabIndex = 45;
            // 
            // identifier
            // 
            this.identifier.Enabled = false;
            this.identifier.Location = new System.Drawing.Point(6, 73);
            this.identifier.Name = "identifier";
            this.identifier.Size = new System.Drawing.Size(132, 20);
            this.identifier.TabIndex = 7;
            this.identifier.TabStop = false;
            // 
            // quotesRadioButton
            // 
            this.quotesRadioButton.AutoSize = true;
            this.quotesRadioButton.Location = new System.Drawing.Point(8, 52);
            this.quotesRadioButton.Name = "quotesRadioButton";
            this.quotesRadioButton.Size = new System.Drawing.Size(59, 17);
            this.quotesRadioButton.TabIndex = 6;
            this.quotesRadioButton.Text = "Quotes";
            this.quotesRadioButton.UseVisualStyleBackColor = true;
            this.quotesRadioButton.CheckedChanged += new System.EventHandler(this.quotesRadioButton_CheckedChanged);
            // 
            // ratesRadioButton
            // 
            this.ratesRadioButton.AutoSize = true;
            this.ratesRadioButton.Checked = true;
            this.ratesRadioButton.Location = new System.Drawing.Point(8, 3);
            this.ratesRadioButton.Name = "ratesRadioButton";
            this.ratesRadioButton.Size = new System.Drawing.Size(53, 17);
            this.ratesRadioButton.TabIndex = 40;
            this.ratesRadioButton.TabStop = true;
            this.ratesRadioButton.Text = "Rates";
            this.ratesRadioButton.UseVisualStyleBackColor = true;
            this.ratesRadioButton.CheckedChanged += new System.EventHandler(this.ratesRadioButton_CheckedChanged);
            // 
            // rateTypesComboBox
            // 
            this.rateTypesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.rateTypesComboBox.FormattingEnabled = true;
            this.rateTypesComboBox.ItemHeight = 13;
            this.rateTypesComboBox.Location = new System.Drawing.Point(6, 25);
            this.rateTypesComboBox.Name = "rateTypesComboBox";
            this.rateTypesComboBox.Size = new System.Drawing.Size(134, 21);
            this.rateTypesComboBox.TabIndex = 5;
            this.rateTypesComboBox.TabStop = false;
            // 
            // toDate
            // 
            this.toDate.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.toDate.Location = new System.Drawing.Point(6, 158);
            this.toDate.MinDate = new System.DateTime(1995, 1, 1, 0, 0, 0, 0);
            this.toDate.Name = "toDate";
            this.toDate.Size = new System.Drawing.Size(135, 20);
            this.toDate.TabIndex = 9;
            this.toDate.TabStop = false;
            // 
            // fromDate
            // 
            this.fromDate.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.fromDate.Location = new System.Drawing.Point(6, 112);
            this.fromDate.MinDate = new System.DateTime(1995, 1, 1, 0, 0, 0, 0);
            this.fromDate.Name = "fromDate";
            this.fromDate.Size = new System.Drawing.Size(135, 20);
            this.fromDate.TabIndex = 8;
            this.fromDate.TabStop = false;
            this.fromDate.Value = new System.DateTime(1995, 1, 1, 0, 0, 0, 0);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.ForeColor = System.Drawing.Color.DarkBlue;
            this.label15.Location = new System.Drawing.Point(5, 94);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(30, 13);
            this.label15.TabIndex = 39;
            this.label15.Text = "From";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.ForeColor = System.Drawing.Color.DarkBlue;
            this.label18.Location = new System.Drawing.Point(2, 139);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(20, 13);
            this.label18.TabIndex = 37;
            this.label18.Text = "To";
            // 
            // webServiceRadioButton
            // 
            this.webServiceRadioButton.AutoSize = true;
            this.webServiceRadioButton.Location = new System.Drawing.Point(5, 181);
            this.webServiceRadioButton.Name = "webServiceRadioButton";
            this.webServiceRadioButton.Size = new System.Drawing.Size(117, 17);
            this.webServiceRadioButton.TabIndex = 4;
            this.webServiceRadioButton.Text = "Using Web Service";
            this.webServiceRadioButton.UseVisualStyleBackColor = true;
            this.webServiceRadioButton.CheckedChanged += new System.EventHandler(this.webServiceRadioButton_CheckedChanged);
            // 
            // infileRadioButton
            // 
            this.infileRadioButton.AutoSize = true;
            this.infileRadioButton.Checked = true;
            this.infileRadioButton.Location = new System.Drawing.Point(5, 70);
            this.infileRadioButton.Name = "infileRadioButton";
            this.infileRadioButton.Size = new System.Drawing.Size(104, 17);
            this.infileRadioButton.TabIndex = 1;
            this.infileRadioButton.TabStop = true;
            this.infileRadioButton.Text = "Using a CSV File";
            this.infileRadioButton.UseVisualStyleBackColor = true;
            this.infileRadioButton.CheckedChanged += new System.EventHandler(this.infileRadioButton_CheckedChanged);
            // 
            // filePanel
            // 
            this.filePanel.Controls.Add(this.label20);
            this.filePanel.Controls.Add(this.irBrowseButton);
            this.filePanel.Controls.Add(this.filePathTextBox);
            this.filePanel.Location = new System.Drawing.Point(19, 93);
            this.filePanel.Name = "filePanel";
            this.filePanel.Size = new System.Drawing.Size(141, 82);
            this.filePanel.TabIndex = 42;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.ForeColor = System.Drawing.Color.DarkBlue;
            this.label20.Location = new System.Drawing.Point(3, 7);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(87, 13);
            this.label20.TabIndex = 33;
            this.label20.Text = "Browse Data File";
            // 
            // irBrowseButton
            // 
            this.irBrowseButton.Location = new System.Drawing.Point(64, 52);
            this.irBrowseButton.Name = "irBrowseButton";
            this.irBrowseButton.Size = new System.Drawing.Size(75, 23);
            this.irBrowseButton.TabIndex = 3;
            this.irBrowseButton.TabStop = false;
            this.irBrowseButton.Text = "Browse";
            this.irBrowseButton.UseVisualStyleBackColor = true;
            this.irBrowseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // filePathTextBox
            // 
            this.filePathTextBox.Location = new System.Drawing.Point(6, 26);
            this.filePathTextBox.Name = "filePathTextBox";
            this.filePathTextBox.Size = new System.Drawing.Size(135, 20);
            this.filePathTextBox.TabIndex = 2;
            this.filePathTextBox.TabStop = false;
            // 
            // updateButton
            // 
            this.updateButton.Location = new System.Drawing.Point(85, 390);
            this.updateButton.Name = "updateButton";
            this.updateButton.Size = new System.Drawing.Size(75, 23);
            this.updateButton.TabIndex = 11;
            this.updateButton.TabStop = false;
            this.updateButton.Text = "Update";
            this.updateButton.UseVisualStyleBackColor = true;
            this.updateButton.Click += new System.EventHandler(this.updateButton_Click);
            // 
            // deleteButton
            // 
            this.deleteButton.Location = new System.Drawing.Point(4, 390);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(75, 23);
            this.deleteButton.TabIndex = 10;
            this.deleteButton.TabStop = false;
            this.deleteButton.Text = "Delete";
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // tablsComboBox
            // 
            this.tablsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tablsComboBox.FormattingEnabled = true;
            this.tablsComboBox.Location = new System.Drawing.Point(9, 41);
            this.tablsComboBox.Name = "tablsComboBox";
            this.tablsComboBox.Size = new System.Drawing.Size(150, 21);
            this.tablsComboBox.TabIndex = 0;
            this.tablsComboBox.TabStop = false;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.ForeColor = System.Drawing.Color.DarkBlue;
            this.label11.Location = new System.Drawing.Point(6, 23);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(67, 13);
            this.label11.TabIndex = 30;
            this.label11.Text = "Select Table";
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.AliceBlue;
            this.tabPage1.Controls.Add(this.groupBox4);
            this.tabPage1.ForeColor = System.Drawing.Color.DarkBlue;
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(167, 504);
            this.tabPage1.TabIndex = 3;
            this.tabPage1.Text = "Jobs";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.endJobButton);
            this.groupBox4.Controls.Add(this.listView1);
            this.groupBox4.Location = new System.Drawing.Point(0, 3);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(165, 495);
            this.groupBox4.TabIndex = 33;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Job Manager";
            // 
            // endJobButton
            // 
            this.endJobButton.Location = new System.Drawing.Point(84, 464);
            this.endJobButton.Name = "endJobButton";
            this.endJobButton.Size = new System.Drawing.Size(75, 23);
            this.endJobButton.TabIndex = 1;
            this.endJobButton.TabStop = false;
            this.endJobButton.Text = "End Job";
            this.endJobButton.UseVisualStyleBackColor = true;
            this.endJobButton.Click += new System.EventHandler(this.endJobButton_Click);
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7});
            this.listView1.FullRowSelect = true;
            this.listView1.Location = new System.Drawing.Point(3, 19);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(152, 431);
            this.listView1.TabIndex = 0;
            this.listView1.TabStop = false;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "ID";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Model";
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "D/M";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Time";
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Simulations";
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Nodes";
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "Data";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(84, 148);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 37;
            this.button1.Text = "Update";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(4, 148);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 36;
            this.button2.Text = "Delete";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // comboBox2
            // 
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(9, 41);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(150, 21);
            this.comboBox2.TabIndex = 35;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(9, 93);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(150, 20);
            this.textBox2.TabIndex = 34;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.ForeColor = System.Drawing.Color.DarkBlue;
            this.label8.Location = new System.Drawing.Point(6, 72);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(87, 13);
            this.label8.TabIndex = 33;
            this.label8.Text = "Browse Data File";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(84, 119);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 32;
            this.button3.Text = "Browse";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.BackColor = System.Drawing.Color.Transparent;
            this.label9.ForeColor = System.Drawing.Color.DarkBlue;
            this.label9.Location = new System.Drawing.Point(6, 23);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(67, 13);
            this.label9.TabIndex = 30;
            this.label9.Text = "Select Table";
            // 
            // Finance
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabWindow);
            this.Name = "Finance";
            this.Size = new System.Drawing.Size(175, 530);
            this.Load += new System.EventHandler(this.Finance_Load);
            this.tabWindow.ResumeLayout(false);
            this.tabForecast.ResumeLayout(false);
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabData.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.wsPanel.ResumeLayout(false);
            this.wsPanel.PerformLayout();
            this.filePanel.ResumeLayout(false);
            this.filePanel.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.TabControl tabWindow;
        private System.Windows.Forms.TabPage tabForecast;
        private System.Windows.Forms.ComboBox modelComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton usingParaRadioButton;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox tablesComboBox;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox paraTextBox;
        private System.Windows.Forms.RadioButton usingDataRadioButton;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox paraFormatTextBox;
        private System.Windows.Forms.TabPage tabData;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.TextBox tableNameTextBox;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button updateButton;
        private System.Windows.Forms.Button deleteButton;
        private System.Windows.Forms.ComboBox tablsComboBox;
        private System.Windows.Forms.TextBox filePathTextBox;
        private System.Windows.Forms.Button irBrowseButton;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Panel wsPanel;
        private System.Windows.Forms.DateTimePicker toDate;
        private System.Windows.Forms.DateTimePicker fromDate;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.RadioButton webServiceRadioButton;
        private System.Windows.Forms.RadioButton infileRadioButton;
        private System.Windows.Forms.Panel filePanel;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.ComboBox rateTypesComboBox;
        private System.Windows.Forms.TextBox identifier;
        private System.Windows.Forms.RadioButton quotesRadioButton;
        private System.Windows.Forms.RadioButton ratesRadioButton;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox periodTextBox;
        private System.Windows.Forms.ComboBox periodTypeComboBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button executeButton;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.Button endJobButton;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ComboBox periodTypeComboBox2;
    }
}
