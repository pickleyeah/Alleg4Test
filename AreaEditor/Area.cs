using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AreaEditor
{
    public class Area
    {
        public Area() { }
        public Area(int width_, int height_)
        {
            width = width_;
            height = height_;
            Blocks = new Block[width_ * height_];
            for (int i = 0; i < Blocks.Length; i++)
                Blocks[i] = new Block();
            Entities = new List<Entity>();
        }
        [XmlAttribute]
        public int width { get; set; }
        [XmlAttribute]
        public int height { get; set; }

        public Block[] Blocks { get; set; }

        public Block GetBlock(int x, int y)
        {
            return Blocks[y * width + x];
        }

        public List<Entity> Entities { get; set; }
    }

    public class Block
    {
        public Block()
        {
            sprite = "Data/Tiles/Grass.png";
        }
        [XmlAttribute]
        public int flags { get; set; }
        [XmlAttribute]
        public string sprite { get; set; }

        public WarpDetails Warp { get; set; }
    }

    public class WarpDetails
    {
        public string Area { get; set; }
        public Vec2 Pos { get; set; }
        public int Dir { get; set; }
    }
}
