using System;
using System.Numerics;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

BenchmarkRunner.Run<BigIntegerBenchmarks>();

public class BigIntegerBenchmarks
{
    BigInteger a = BigInteger.Parse("-1234567890123456789012345678901234567890");
    BigInteger b = BigInteger.Parse("9876543210987654321098765432109876543210");
    [Benchmark]
    public void Addition_SystemBigInteger()
    {
        BigInteger result = a + b;
    }
    [Benchmark]
    public void Multiplication_SystemBigInteger()
    {
        BigInteger result = a * b;
    }
    [Benchmark]
    public void Substraction_SystemBigInteger()
    {
        BigInteger result = a - b;
    }
    [Benchmark]
    public void Division_SystemBigInteger()
    {
        BigInteger result = a / b;
    }
    
}
