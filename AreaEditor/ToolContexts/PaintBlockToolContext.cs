using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class PaintBlockToolContext : ToolContext
    {
        private Image m_image;
        private string m_imageName;
        private bool m_mouseInArea, m_mouseDown;
        private Point m_pos;
        private Pen m_pen = new Pen(Color.Black);

        public PaintBlockToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }

        public override void ImageClicked(string imageName, System.Drawing.Image image, Area area)
        {
            m_image = image;
            m_imageName = imageName;
        }

        public override void MouseEnter(object sender, EventArgs e, Area area)
        {
            m_mouseInArea = true;
            EditorSurface.Invalidate();
        }

        public override void MouseLeave(object sender, EventArgs e, Area area)
        {
            m_mouseInArea = false;
            EditorSurface.Invalidate();
        }

        public override void MouseDown(object sender, MouseEventArgs e, Area area)
        {
            if (e.Button == MouseButtons.Left)
            {
                m_mouseDown = true;
                // Trigger a mousemove event so clicking without moving will still set the block image
                MouseMove(sender, e, area);
            }
        }

        public override void MouseUp(object sender, MouseEventArgs e, Area area)
        {
            m_mouseDown = false;
        }

        public override void MouseMove(object sender, MouseEventArgs e, Area area)
        {
            m_pos = e.Location;

            if (m_image == null)
                return;
            // TODO: update block sprite name
            if (m_mouseDown)
            {
                var block = area.GetBlock(m_pos.X / Area.BlockSize, m_pos.Y / Area.BlockSize);
                if (block.sprite != m_imageName)
                {
                    block.sprite = m_imageName;
                }
            }

            EditorSurface.Invalidate();
        }

        public override void Paint(object sender, PaintEventArgs e, Area area)
        {
            if (m_mouseInArea && m_image != null)
            {
                e.Graphics.DrawImageUnscaled(m_image, m_pos);
                e.Graphics.DrawRectangle(m_pen, m_pos.X, m_pos.Y, Area.BlockSize, Area.BlockSize);
            }
        }
    }
}
