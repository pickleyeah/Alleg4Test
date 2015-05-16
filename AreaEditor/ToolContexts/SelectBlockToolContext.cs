using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AreaEditor.ToolContexts
{
    public class SelectBlockToolContext : ToolContext
    {
        private List<Point> m_selectedBlocks = new List<Point>();
        private bool m_mouseDown = false;
        private Rectangle m_selection;
        private Point m_initialMousePos, m_currentMousePos;
        private Brush m_brush = new SolidBrush(Color.FromArgb(96, Color.Blue));

        public SelectBlockToolContext(PropertyGrid grid, Control surface) :
            base(grid, surface)
        {
        }

        public override void MouseDown(object sender, MouseEventArgs e, Area area)
        {
            if (e.Button != MouseButtons.Left)
                return;
            m_mouseDown = true;
            m_initialMousePos = e.Location;
            m_currentMousePos = e.Location;
            m_selection = new Rectangle(e.Location, new Size());
            EditorSurface.Invalidate();
        }

        public override void MouseMove(object sender, MouseEventArgs e, Area area)
        {
            m_currentMousePos = e.Location;
            int x = Math.Min(m_initialMousePos.X, m_currentMousePos.X);
            int y = Math.Min(m_initialMousePos.Y, m_currentMousePos.Y);
            int w = Math.Abs(m_currentMousePos.X - m_initialMousePos.X);
            int h = Math.Abs(m_currentMousePos.Y - m_initialMousePos.Y);
            m_selection = new Rectangle(x, y, w, h);
            EditorSurface.Invalidate();
        }

        private Rectangle ConvertFromSelectionToBlockRange(Rectangle selection)
        {
            Rectangle result = selection;
            if (result.X != 0)
                result.X /= Area.BlockSize;
            if (result.Y != 0)
                result.Y /= Area.BlockSize;
            if (result.Width != 0)
                result.Width /= Area.BlockSize;
            if (result.Height != 0)
                result.Height /= Area.BlockSize;
            return result;
        }

        public override void MouseUp(object sender, MouseEventArgs e, Area area)
        {
            if (e.Button != MouseButtons.Left)
                return;
            m_mouseDown = false;
            m_currentMousePos = e.Location;

            int x = e.X / Area.BlockSize;
            int y = e.Y / Area.BlockSize;
            if (x < 0 || x >= area.width || y < 0 || y >= area.height)
                return;

            if (Control.ModifierKeys == Keys.Control)
            {
                // Toggle/add single block
                if (!m_selectedBlocks.Remove(new Point(x, y)))
                    m_selectedBlocks.Add(new Point(x, y));
            }
            else if ((Control.ModifierKeys & Keys.Shift) > 0)
            {
                if ((Control.ModifierKeys & Keys.Control) == 0)
                    m_selectedBlocks.Clear();
                Rectangle blockRange = ConvertFromSelectionToBlockRange(m_selection);
                for (int j = blockRange.Y; j <= blockRange.Bottom; j++)
                {
                    for (int i = blockRange.X; i <= blockRange.Right; i++)
                    {
                        if (!m_selectedBlocks.Remove(new Point(i, j)))
                            m_selectedBlocks.Add(new Point(i, j));
                    }
                }
            }
            else // Single block selection
            {
                m_selectedBlocks.Clear();
                m_selectedBlocks.Add(new Point(x, y));
            }

            EditorGrid.SelectedObjects = m_selectedBlocks.Select(p => area.GetBlock(p.X, p.Y)).ToArray();
            EditorSurface.Invalidate();
        }

        public override void Paint(object sender, PaintEventArgs e, Area area)
        {
            // Draw all the currently selected blocks
            foreach (var point in m_selectedBlocks)
            {
                Rectangle rect = new Rectangle(new Point(point.X * Area.BlockSize, point.Y * Area.BlockSize), new Size(Area.BlockSize, Area.BlockSize));
                e.Graphics.FillRectangle(m_brush, rect);
            }
            if (m_mouseDown) // Draw the estimated selection
            {
                if ((Control.ModifierKeys & Keys.Shift) > 0) // Shift = multi-select
                {
                    e.Graphics.FillRectangle(m_brush, m_selection);
                }
                else // Highlight the block under the mouse
                {
                    int x = m_currentMousePos.X / Area.BlockSize;
                    int y = m_currentMousePos.Y / Area.BlockSize;
                    if (x < 0 || x >= area.width || y < 0 || y >= area.height)
                        return;
                    e.Graphics.FillRectangle(m_brush, x * Area.BlockSize, y * Area.BlockSize, Area.BlockSize, Area.BlockSize);
                }
            }
        }

        public override void ImageClicked(string imageName, Image image, Area area)
        {
            // Fill all the currently selected block's sprite with the new image
            if (m_selectedBlocks.Count == 0)
                return;
            foreach (var p in m_selectedBlocks)
            {
                area.GetBlock(p.X, p.Y).sprite = imageName;
                area.GetBlock(p.X, p.Y).CachedBlockImage = image;
            }
            EditorSurface.Invalidate();
        }
    }
}
