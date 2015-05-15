using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor
{
    public partial class frmNewDialog : Form
    {
        public int AreaWidth { get; set; }
        public int AreaHeight { get; set; }

        public frmNewDialog()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (ValidateFields())
            {
                DialogResult = System.Windows.Forms.DialogResult.OK;
                return;
            }
            MessageBox.Show("Invalid parameters specified!");
        }

        private bool ValidateFields()
        {
            int w, h;
            if (!int.TryParse(txtWidth.Text, out w) || !int.TryParse(txtHeight.Text, out h))
                return false;
            AreaWidth = w;
            AreaHeight = h;
            return true;
        }
    }
}
