using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class AddWarpToolContext : ToolContext
    {
        private Brush m_brush = new SolidBrush(Color.FromArgb(96, Color.Green));

        public AddWarpToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }

        public override void MouseDown(object sender, MouseEventArgs e, Area area)
        {
            if (e.Button == MouseButtons.Left)
            {
                int x = e.X / Area.BlockSize;
                int y = e.Y / Area.BlockSize;
                if (x < 0 || y < 0 || x >= area.width || y >= area.height)
                    return;

                var block = area.GetBlock(x, y);
                using (var form = new frmAddWarp(block))
                {
                    form.ShowDialog();
                }
                EditorSurface.Invalidate();
            }
        }

        public override void Paint(object sender, PaintEventArgs e, Area area)
        {
            // Paint the blocks with warp points green
            for (int y = 0; y < area.height; y++)
            {
                for (int x = 0; x < area.width; x++)
                {
                    if (area.GetBlock(x,y).Warp != null)
                    {
                        e.Graphics.FillRectangle(m_brush, x * Area.BlockSize, y * Area.BlockSize, Area.BlockSize, Area.BlockSize);
                    }
                }
            }
        }
    }
}
