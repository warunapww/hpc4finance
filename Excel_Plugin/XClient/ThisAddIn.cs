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
using System.Windows.Forms;
using Microsoft.VisualStudio.Tools.Applications.Runtime;
using Excel = Microsoft.Office.Interop.Excel;
using Office = Microsoft.Office.Core;
using Microsoft.Office.Tools;
using Microsoft.Office.Core;

namespace XClient
{
    public partial class ThisAddIn
    {
        Finance fControl;
        Microsoft.Office.Tools.CustomTaskPane myPane;

        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            #region VSTO generated code

            this.Application = (Excel.Application)Microsoft.Office.Tools.Excel.ExcelLocale1033Proxy.Wrap(typeof(Excel.Application), this.Application);

            #endregion
            fControl = new Finance();
            myPane = this.CustomTaskPanes.Add(fControl, "Finance");
            myPane.Visible = true;

        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
        }

        #region VSTO generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InternalStartup()
        {
            this.Startup += new System.EventHandler(ThisAddIn_Startup);
            this.Shutdown += new System.EventHandler(ThisAddIn_Shutdown);
        }

        #endregion


    }
}
