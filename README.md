
# dmarc

Tools to Machinate DMARC (WIP)

## Description

(this totally needs a ‘jalad’ function)

## What’s Inside The Tin

The following functions are implemented:

  - `is_dmarc_valid`: Test if DMARC records are valid `read_dmarc`: Read
    DMARC records for a set of domains

## Installation

``` r
devtools::install_github("hrbrmstr/dmarc")
```

## Usage

``` r
library(dmarc)

# current verison
packageVersion("dmarc")
```

    ## [1] '0.1.0'

``` r
library(gdns)
library(tidyverse)

doms <- c("linkedin.com", "twitter.com", "google.com", "rud.is", "facebook.com")

# getting dmarc records on our own (note that "rud.is" has no DMARC and is not in the results of the `bulk_query()` call)
sprintf("_dmarc.%s", doms) %>% 
  gdns::bulk_query("TXT") -> dmarc_recs

dmarc_recs
```

    ##                   name type  TTL
    ## 1 _dmarc.linkedin.com.   16 3599
    ## 2  _dmarc.twitter.com.   16  299
    ## 3   _dmarc.google.com.   16  299
    ## 4 _dmarc.facebook.com.   16 3599
    ##                                                                                                                    data
    ## 1                                 "v=DMARC1; p=reject; rua=mailto:d@rua.agari.com; ruf=mailto:d@ruf.agari.com; pct=100"
    ## 2                                    "v=DMARC1; p=reject; rua=mailto:d@rua.agari.com; ruf=mailto:d@ruf.agari.com; fo=1"
    ## 3                                                          "v=DMARC1; p=reject; rua=mailto:mailauth-reports@google.com"
    ## 4 "v=DMARC1; p=reject; pct=100; rua=mailto:d@rua.agari.com,mailto:postmaster@facebook.com; ruf=mailto:d@ruf.agari.com;"

``` r
glimpse(dmarc_recs)
```

    ## Observations: 4
    ## Variables: 4
    ## $ name <chr> "_dmarc.linkedin.com.", "_dmarc.twitter.com.", "_dmarc.google.com.", "_dmarc.facebook.com."
    ## $ type <int> 16, 16, 16, 16
    ## $ TTL  <int> 3599, 299, 299, 3599
    ## $ data <chr> "\"v=DMARC1; p=reject; rua=mailto:d@rua.agari.com; ruf=mailto:d@ruf.agari.com; pct=100\"", "\"v=DMARC1...

``` r
is_dmarc_valid(doms, pull(dmarc_recs, data))
```

    ## [1] TRUE TRUE TRUE TRUE

``` r
# using the library to get details (WIP)
read_dmarc(c("linkedin.com", "twitter.com", "google.com", "rud.is", "facebook.com"))
```

    ##   has_valid_dmarc  dkim_domain pct adkim aspf   p sp                                                   rua
    ## 1            TRUE linkedin.com 100   114  114 114  0                                mailto:d@rua.agari.com
    ## 2            TRUE  twitter.com 100   114  114 114  0                                mailto:d@rua.agari.com
    ## 3            TRUE   google.com 100   114  114 114  0                    mailto:mailauth-reports@google.com
    ## 4           FALSE               NA    NA   NA  NA NA                                                      
    ## 5            TRUE facebook.com 100   114  114 114  0 mailto:d@rua.agari.com,mailto:postmaster@facebook.com
    ##                      ruf
    ## 1 mailto:d@ruf.agari.com
    ## 2 mailto:d@ruf.agari.com
    ## 3                       
    ## 4                       
    ## 5 mailto:d@ruf.agari.com
