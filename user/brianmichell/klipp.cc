/* A Hello World example of opening an MDIO file.
 * 
 * https://ahay.org/wiki/Guide_to_madagascar_API#C++_interface
 * Clip the data.
 * This program reads an RSF input and for each trace clips sample values
 * that exceed the given threshold (and similarly clips values below
 * the negative threshold). The threshold is passed via the "clip" parameter.
 */

#include <valarray>
#include <rsf.hh>

#ifdef NO_MDIO
#error "Madagascar API not built with MDIO support; disable MDIO dependent code or rebuild API with MDIO"
#else
#include <mdio/mdio.h>
#endif

int main(int argc, char* argv[])
{

    #ifndef NO_MDIO
    // BEGIN HELLO WORLD MDIO
    std::string path = "s3://tgs-opendata-poseidon/full_stack_agc.mdio";

    mdio::Future<mdio::Dataset> dsRes = mdio::Dataset::Open(path, mdio::constants::kOpen);
    if (!dsRes.status().ok()) {
        std::cerr << "Failed to open dataset: " << dsRes.status() << std::endl;
        return 1;
    }

    mdio::Dataset ds = dsRes.value();
    std::cout << ds << std::endl;
    // END HELLO WORLD MDIO
    #endif
    
    sf_init(argc, argv); // Initialize RSF

    iRSF par(0), in; // Input parameter and file
    oRSF out;        // Output file

    int n1, n2;      // Trace length and number of traces
    float clip;
    
    in.get("n1", n1);
    n2 = in.size(1);

    par.get("clip", clip); // Parameter from the command line

    std::valarray<float> trace(n1);

    for (int i2 = 0; i2 < n2; i2++) { // Loop over traces
        in >> trace; // Read a trace

        for (int i1 = 0; i1 < n1; i1++) { // Loop over samples
            if (trace[i1] > clip)
                trace[i1] = clip;
            else if (trace[i1] < -clip)
                trace[i1] = -clip;
        }

        out << trace; // Write the clipped trace
    }

    return 0;
} 