/// Minimal complex number type for internal ITM calculations.
#[derive(Clone, Copy, Debug, Default)]
pub struct Complex {
    pub re: f64,
    pub im: f64,
}

impl Complex {
    #[inline]
    pub fn new(re: f64, im: f64) -> Self {
        Complex { re, im }
    }

    /// Magnitude (absolute value)
    #[inline]
    pub fn abs(self) -> f64 {
        (self.re * self.re + self.im * self.im).sqrt()
    }

    /// Complex square root (principal branch)
    #[inline]
    pub fn sqrt(self) -> Self {
        let r = self.abs();
        let re = ((r + self.re) / 2.0).sqrt();
        let im = if self.im >= 0.0 {
            ((r - self.re) / 2.0).sqrt()
        } else {
            -((r - self.re) / 2.0).sqrt()
        };
        Complex::new(re, im)
    }
}

impl std::ops::Add for Complex {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        Complex::new(self.re + rhs.re, self.im + rhs.im)
    }
}

impl std::ops::Sub for Complex {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        Complex::new(self.re - rhs.re, self.im - rhs.im)
    }
}

impl std::ops::Mul for Complex {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        Complex::new(
            self.re * rhs.re - self.im * rhs.im,
            self.re * rhs.im + self.im * rhs.re,
        )
    }
}

impl std::ops::Div for Complex {
    type Output = Self;
    fn div(self, rhs: Self) -> Self {
        let denom = rhs.re * rhs.re + rhs.im * rhs.im;
        Complex::new(
            (self.re * rhs.re + self.im * rhs.im) / denom,
            (self.im * rhs.re - self.re * rhs.im) / denom,
        )
    }
}

// Complex - f64
impl std::ops::Sub<f64> for Complex {
    type Output = Self;
    fn sub(self, rhs: f64) -> Self {
        Complex::new(self.re - rhs, self.im)
    }
}

// f64 - Complex
impl std::ops::Sub<Complex> for f64 {
    type Output = Complex;
    fn sub(self, rhs: Complex) -> Complex {
        Complex::new(self - rhs.re, -rhs.im)
    }
}

// f64 + Complex
impl std::ops::Add<Complex> for f64 {
    type Output = Complex;
    fn add(self, rhs: Complex) -> Complex {
        Complex::new(self + rhs.re, rhs.im)
    }
}

// Complex * f64
impl std::ops::Mul<f64> for Complex {
    type Output = Self;
    fn mul(self, rhs: f64) -> Self {
        Complex::new(self.re * rhs, self.im * rhs)
    }
}

// f64 * Complex
impl std::ops::Mul<Complex> for f64 {
    type Output = Complex;
    fn mul(self, rhs: Complex) -> Complex {
        Complex::new(self * rhs.re, self * rhs.im)
    }
}

// Complex / f64
impl std::ops::Div<f64> for Complex {
    type Output = Self;
    fn div(self, rhs: f64) -> Self {
        Complex::new(self.re / rhs, self.im / rhs)
    }
}
