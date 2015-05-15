using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace AreaEditor
{
    [TypeConverter(typeof(Vec2Converter))]
    public class Vec2
    {
        public float x { get; set; }
        public float y { get; set; }
    }

    public class Vec2Converter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(Vec2))
                return true;
            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(System.String) && value is Vec2)
                return ((Vec2)value).x + ", " + ((Vec2)value).y;
            return base.ConvertTo(context, culture, value, destinationType);
        }
    }
}
