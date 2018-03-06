#!/bin/sh

prop_file_ret=""
read_prop_file () {
    if [ $# -ne 2 ];then
        echo "you must put one parmes"
        exit 1
    fi
    
    file=$1
    key=$2
    
    value=`awk -F= -v k=${key} '{ if ( $1 == k ) print $2; }' $file`
    if [ $? -ne 0 ];then
        echo " para config.properties file failed"
        exit 1
    else
        if [ "x${value}" = "x" ];then
            exit 1
        fi
    fi

    prop_file_ret=$value
    return 0
} 

export ORIGDIR=$(cd `dirname $0`; cd ..; pwd)
export GOPATH=/tmp/pkrss/gopath
export GOROOT=/tmp/pkrss/go
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin

if [ ! -d "/tmp/pkrss/go" ];then
    echo 'go is not installed, then install go lang'
    mkdir -p /tmp/pkrss/go ~/tmp
    
    wget https://redirector.gvt1.com/edgedl/go/go1.9.2.linux-amd64.tar.gz -O /tmp/pkrss/go.tar.gz
    
    tar -zxf /tmp/pkrss/go.tar.gz -C /tmp/pkrss

    # cd $GOPATH/

    # mkdir -p $GOPATH/src/golang.org/

    # git clone https://github.com/zieckey/golang.org.git $GOPATH/src/golang.org/

    go get -v github.com/axgle/mahonia github.com/hprose/hprose-golang

    # cd $ORIGDIR
else
    echo 'go is installed, skip install go lang'
fi

mkdir -p $GOPATH/src/sx98

read_prop_file "$ORIGDIR/rdc_security_config.properties" "giturl"
giturl=$prop_file_ret
git clone $giturl $GOPATH/src/hx98/

mv api/Dockerfile $ORIGDIR/
mv api/conf.d $ORIGDIR/
mv admin $GOPATH/src/sx98/
mv api $GOPATH/src/sx98/
mv base $GOPATH/src/sx98/
mv contract $GOPATH/src/sx98/
mv sys $GOPATH/src/sx98/

cd $GOPATH/src/sx98/api

CGO_ENABLED=0 go build -a .

# npm --registry=https://registry.npm.taobao.org install grunt 
# npm --registry=https://registry.npm.taobao.org install 
# node_modules/grunt/bin/grunt publish

mkdir dist
/bin/cp -rf static dist/static

cd $ORIGDIR

mv $GOPATH/src/sx98/api/api ./app
mv $GOPATH/src/sx98/api/conf.d ./
mv $GOPATH/src/sx98/api/conf ./
mv $GOPATH/src/sx98/api/views ./
mv $GOPATH/src/sx98/api/dist ./

rm -rf $GOPATH/src/sx98 $GOPATH/src/hx98
