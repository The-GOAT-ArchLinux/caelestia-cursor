# Maintainer: André Soares <345001834+The-GOAT-ArchLinux@users.noreply.github.com>
pkgname=caelestia-cursor
pkgver=1.0.0
pkgrel=1
pkgdesc="CLI tool to view and change the cursor theme and size on a Caelestia (Hyprland) setup"
arch=('x86_64' 'aarch64')
license=('MIT')
depends=('gcc-libs' 'hyprland')
makedepends=('gcc')
optdepends=('glib2: sync the GTK cursor theme through gsettings')
source=("$pkgname.cpp")
sha256sums=('SKIP')

build() {
    g++ -std=c++17 -O2 -Wall -o "$pkgname" "$pkgname.cpp"
}

package() {
    install -Dm755 "$pkgname" "$pkgdir/usr/bin/$pkgname"
}
