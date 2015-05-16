using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AreaEditor
{
    public partial class frmAddWarp : Form
    {
        public Block CurrentBlock { get; set; }

        public frmAddWarp(Block block)
        {
            InitializeComponent();
            cboDir.Items.AddRange(Enum.GetNames(typeof(DIR)));
            cboDir.SelectedIndex = 0;

            CurrentBlock = block;
            if (CurrentBlock.Warp != null)
            {
                txtAreaName.Text = CurrentBlock.Warp.Area;
                txtStartPosX.Text = CurrentBlock.Warp.Pos.x.ToString();
                txtStartPosY.Text = CurrentBlock.Warp.Pos.y.ToString();
                cboDir.SelectedIndex = CurrentBlock.Warp._dir;
            }
        }

        public bool ValidateInput()
        {
            int startX, startY;
            if (!int.TryParse(txtStartPosX.Text, out startX) || !int.TryParse(txtStartPosY.Text, out startY))
                return false;

            CurrentBlock.Warp = new WarpDetails();
            CurrentBlock.Warp.Dir = (DIR)cboDir.SelectedIndex;
            CurrentBlock.Warp.Area = txtAreaName.Text;
            CurrentBlock.Warp.Pos = new Vec2() { x = startX, y = startY };
            return true;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (!ValidateInput())
                return;
            DialogResult = System.Windows.Forms.DialogResult.OK;
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            CurrentBlock.Warp = null;
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }
    }
}
