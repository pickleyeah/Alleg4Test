using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class AddWarpToolContext : ToolContext
    {
        public AddWarpToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }

        public override void MouseDown(object sender, MouseEventArgs e, Area area)
        {
            int x = e.X / Area.BlockSize;
            int y = e.Y / Area.BlockSize;
            if (x < 0 || y < 0 || x >= area.width || y >= area.height)
                return;

            var block = area.GetBlock(x,y);
            using (var form = new frmAddWarp(block))
            {
                form.ShowDialog();
            }
        }
    }
}
