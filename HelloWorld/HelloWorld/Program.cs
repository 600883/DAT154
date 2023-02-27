
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HelloWorld
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                var a = 1;
                var b = 2;
                var c = 3;
                Console.WriteLine(c > b || c == a);
            }
            catch (Exception)
            {
                Console.WriteLine("the number could not be converted to a byte");
            }
            
        }
    }
}