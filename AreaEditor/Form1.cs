using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;
using AreaEditor.ToolContexts;
using System.Drawing.Imaging;

namespace AreaEditor
{
    public partial class Form1 : Form
    {
        private Area m_area;
        private Point m_cursorImagePos = new Point(0, 0);
        private Image m_image;
        private Dictionary<string, Image> m_imageMap = new Dictionary<string, Image>();

        private string m_filename;
        private ToolStripButton[] m_toolButtons;
        private List<ToolContext> m_toolContexts = new List<ToolContext>();
        private ToolContext m_currentTool;

        private bool m_showGrid = true;
        

        public Form1()
        {
            InitializeComponent();
            areaCanvas.MouseWheel += pictureBox1_MouseWheel;
            InitToolContexts();
            LoadImageMap();
            RefreshTitle();
        }

        private void InitToolContexts()
        {
            m_toolButtons = new ToolStripButton[]
            {
                toolSelectBlock,
                toolPan,
                toolPaintBlock,
                toolPaintBlockOverlay,
                toolAddWarp,
            };

            toolSelectBlock.Tag = new SelectBlockToolContext(propertyGrid1, areaCanvas);
            toolPan.Tag = new PanToolContext(propertyGrid1, areaCanvas);
            toolPaintBlock.Tag = new PaintBlockToolContext(propertyGrid1, areaCanvas);
            toolPaintBlockOverlay.Tag = new PaintBlockOverlayToolContext(propertyGrid1, areaCanvas);
            toolAddWarp.Tag = new AddWarpToolContext(propertyGrid1, areaCanvas);
            
            foreach (var tool in m_toolButtons)
                tool.Click += tool_Click;
            tool_Click(toolSelectBlock, null);
        }

        void tool_Click(object sender, EventArgs e)
        {
            SelectToolButton(sender as ToolStripButton, m_toolButtons);
        }

        void SelectToolButton(ToolStripButton selectedbutton, ToolStripButton[] buttons)
        {
            foreach (var toolButton in buttons)
            {
                toolButton.Checked = (selectedbutton == toolButton);
                if (toolButton.Checked)
                    m_currentTool = (ToolContext)toolButton.Tag;
            }
            areaCanvas.Invalidate();
        }

        private void RefreshTitle()
        {
            Text = string.Format("Area Editor - {0}", !string.IsNullOrEmpty(m_filename) ? Path.GetFileName(m_filename) : "Untitled");
        }

        private void RefreshAreaDisplay()
        {
            areaCanvas.CurrentArea = m_area;
            areaCanvas.Invalidate();
        }

        private void LoadImageMap()
        {
            Directory.CreateDirectory("Data/Tiles");
            foreach (var path in Directory.EnumerateFiles("Data/Tiles", "*.png", SearchOption.AllDirectories))
            {
                string f = path.Replace('\\', '/');   // Get rid of backslashes to make paths consistent between game/editor
                Image tempImage = Image.FromFile(f);
                var image = new Bitmap(tempImage.Width, tempImage.Height, PixelFormat.Format32bppPArgb);
                using (Graphics gr = Graphics.FromImage(image))
                {
                    gr.DrawImage(tempImage, new Rectangle(0, 0, image.Width, image.Height));
                }

                imageList1.Images.Add(f, image);
                m_imageMap.Add(f, image);   // Also keep a dictionary of the images for fast lookup

                ListViewItem item = new ListViewItem();
                item.Text = f;
                item.ImageKey = f;
                listViewTiles.Items.Add(item);
            }
        }

        private void CreateArea(int width, int height)
        {
            m_area = new Area(width, height);
            m_filename = null;
            RefreshAreaDisplay();
            RefreshTitle();
        }

        private bool LoadArea(string filename)
        {
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Area));
                m_area = serializer.Deserialize(new StreamReader(filename)) as Area;
                if (m_area == null)
                    return false;
            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }

        private bool SaveArea(string filename)
        {
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Area));
                using (var writer = new StreamWriter(filename))
                    serializer.Serialize(writer, m_area);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private void splitContainer1_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            propertyGrid1.SelectedObject = m_area;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (frmNewDialog dialog = new frmNewDialog())
            {
                if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    CreateArea(dialog.AreaWidth, dialog.AreaHeight);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog fileDialog = new OpenFileDialog())
            {
                fileDialog.Filter = "XML File (*.xml)|*.xml|All Files (*.*)|*.*";
                if (DialogResult.OK == fileDialog.ShowDialog())
                {
                    if (!LoadArea(fileDialog.FileName))
                    {
                        MessageBox.Show("Unable to open file " + fileDialog.FileName);
                        return;
                    }
                    m_filename = fileDialog.FileName;
                    RefreshAreaDisplay();
                    RefreshTitle();
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_filename))
            {
                saveAsToolStripMenuItem_Click(sender, e);
                return;
            }

            if (!SaveArea(m_filename))
            {
                MessageBox.Show("Unable to save file " + m_filename);
                return;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog fileDialog = new SaveFileDialog())
            {
                fileDialog.Filter = "XML File (*.xml)|*.xml|All Files (*.*)|*.*";
                if (DialogResult.OK == fileDialog.ShowDialog())
                {
                    if (!SaveArea(fileDialog.FileName))
                    {
                        MessageBox.Show("Unable to save file " + fileDialog.FileName);
                        return;
                    }
                    m_filename = fileDialog.FileName;
                    RefreshTitle();
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void listViewTiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            //if (m_area == null)
            //    return;
            //if (listViewTiles.SelectedItems.Count == 1)
            //{
            //    var item = listViewTiles.SelectedItems[0];
            //    if (item != null)
            //        m_currentTool.ImageClicked(item.ImageKey, m_imageMap[item.ImageKey], m_area);
            //}
        }

        #region PictureBox Events
        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseMove(sender, e, m_area);
        }

        private void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseEnter(sender, e, m_area);
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseLeave(sender, e, m_area);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseDown(sender, e, m_area);
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_area == null)
                return;
            m_currentTool.MouseUp(sender, e, m_area);
        }

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {

        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (m_area == null)
                return;
            Graphics g = e.Graphics;
            // Draw Blocks
            for (int y = 0; y < m_area.height; y++)
            {
                for (int x = 0; x < m_area.width; x++)
                {
                    var block = m_area.GetBlock(x, y);
                    // Keep a reference to the image inside the block to actually draw at a decent speed
                    // I assumed imageList1.Images was a hash table but it's actually slow as shit
                    if (block.CachedBlockImage == null)
                        block.CachedBlockImage = m_imageMap[block.sprite];
                    g.DrawImageUnscaled(block.CachedBlockImage, x * Area.BlockSize, y * Area.BlockSize, Area.BlockSize, Area.BlockSize);
                }
            }
            // TODO: Draw entities

            // Draw tool context data
            m_currentTool.Paint(sender, e, m_area);
        }
        #endregion

        private void btnShowGrid_Click(object sender, EventArgs e)
        {
            (sender as ToolStripButton).Checked = !(sender as ToolStripButton).Checked;
            areaCanvas.DrawGrid = (sender as ToolStripButton).Checked;
            areaCanvas.Invalidate();
        }

        private void areaCanvas_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void listViewTiles_DoubleClick(object sender, EventArgs e)
        {
            if (m_area == null)
                return;
            if (listViewTiles.SelectedItems.Count == 1)
            {
                var item = listViewTiles.SelectedItems[0];
                if (item != null)
                    m_currentTool.ImageClicked(item.ImageKey, m_imageMap[item.ImageKey], m_area);
            }
        }
    }
}
