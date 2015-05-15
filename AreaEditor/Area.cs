using System;
using System.Collections.Generic;
using System.ComponentModel;
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
        [XmlAttribute("flags")]
        [ReadOnlyAttribute(true)]
        public int _flags { get; set; }

        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public BlockFlags Flags
        {
            get
            {
                return (BlockFlags)_flags;
            }
            set
            {
                _flags = (int)value;
            }
        }

        [XmlAttribute]
        public string sprite { get; set; }

        [DefaultValue(null)]
        public WarpDetails Warp { get; set; }
    }

    [Flags]
    public enum BlockFlags
    {
        None = 0,
        COLLIDE_NORTH = 0x01,
        COLLIDE_WEST = 0x02,
        COLLIDE_SOUTH = 0x04,
        COLLIDE_EAST = 0x08,
        COLLIDE_ALL = 0x0f,

    };

    public class WarpDetailsConverter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(WarpDetails))
                return true;
            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            return base.ConvertTo(context, culture, value, destinationType);
        }
    }

    [TypeConverterAttribute(typeof(WarpDetailsConverter))]
    public class WarpDetails
    {
        public string Area { get; set; }
        public Vec2 Pos { get; set; }
        public int Dir { get; set; }
    }
}
