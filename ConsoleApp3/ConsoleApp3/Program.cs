namespace ConsoleApp {


      public delegate void Deltest();

    public class A {

        public event Deltest dt;

        

        public A()
    {
            dt += SkrivHei;
           

            dt -= SkrivHei;

            dt?.Invoke();

            
           

            
    }

    public void SkrivHei()
    {
        Console.WriteLine("Hei DAT154!");
    }

    public void SkrivMandag()
        {
             Console.WriteLine("Mandag!");
        }

        public int sum(int a, int b)
        {
            return a + b;
        }

    }

internal class Program
    {
        static void Main(string[] args)
        {
            A a = new A();
           

            a.dt();
        }

        static void SkrivX()
        {
            Console.WriteLine("X");
        }

    }
}