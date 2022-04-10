import _mapper
from enum import Enum
import inspect
import csv

class MapType(Enum):
    CROSSMAP = 1
    EXCLUSIVE = 2
    COLLAPSE = 3


class Mapper():

    def __init__(self, map_type: MapType=MapType.CROSSMAP):
        self.mapper = _mapper.lib.get_map()

        self.map_type = map_type
        self.file_list = {}
        

    ##
    # Set how the crossmapping should run
    #
    #
    def map_type(self, map_type: MapType=MapType.CROSSMAP):
        self.map_type = map_type

    def chrom_column(self, col: int):
        self.chrom_column = col

    # Internal function
    def _file_dialect_inference(self, file_name: str) -> tuple:
        file_type = 'r'
        if self.file_list[file_name]["data_type"] == "binary":
            file_type = 'b'

        # TODO: Try/Except
        with open(file_name, 'r') as csvfile:
            sniffer = csv.Sniffer()
            # TODO: Is 5000 OK no matter what the file size?  Can read in a few lines and pass in as a string instead
            buffer = csvfile.read(5000)
            # TODO: probably don't need to do this
            csvfile.seek(0)

            # Infer if header is present
            self.file_list[file_name]["header"] = sniffer.has_header(buffer)
            
            # Attempt to infer the delimiter
            dialect = sniffer.sniff(buffer)
            self.file_list[file_name]["delimiter"] = dialect.delimiter
            
        
        return (self.file_list[file_name]["header"], self.file_list[file_name]["delimiter"])


    #
    # Can override the inference if a header exists or not in a file
    # File name must already be populated.
    #
    # Return true on success, false on failure.

    def set_header(self, file_name: str, has_header: bool) -> bool:
        if file_name in self.file_list:
            self.file_list[file_name]["header"] = has_header
            return True
        return False

    #
    # Can override the inference of the delimiter in a file
    # File name must already be populated.
    #
    # Return true on success, false on failure.

    def set_header(self, file_name: str, delimiter: str) -> bool:
        if file_name in self.file_list:
            self.file_list[file_name]["delimiter"] = delimiter
            return True
        return False

    #
    # Will register a file as being zero based instead of the default
    # 1 based positioned.
    # File name must already be populated.
    #
    # Return true on success, false on failure.

    def set_zero_based(self, file_name: str) -> bool:
        if file_name in self.file_list:
            self.file_list[file_name]["zero_based"] = True
            return True
        return False

    

    #
    # Returns a list of the current files registered with the instance
    #
    def current_file_list(self) -> list:
        return list(self.file_list.keys())

    #
    # Add a file into the crossmapping.
    #
    # Required to provide the map_column (join column), as well as the start and end
    # positional indexes (can be the same for single position data sets; or can omit the
    # end_index_column in that case).
    #
    # Return value is a tuple of:
    #   inferred header present, inferred delimiter
    # The set_header and set_delimiter functions can be used to correct if the inference is wrong.
    #
    def add_file(self, file_name: str, map_column: int, start_index_column: int, end_index_column: int=-1, zero_based: bool=False) -> tuple:
        sig = inspect.signature(Mapper.add_file)
        # Just put in the signature as a dictionary entry into the file_list
        self.file_list[file_name] = dict(sig.bind(map_column, start_index_column, end_index_column).arguments)
        self.file_list[file_name]["data_type"] = "text"

        # Determine file dialect inferences
        return self._file_dialect_inference(file_name)
        
        

    #
    # Same as add_file, but allows for a binary file to be added
    #
    #
    def add_binary_file(self, file_name: str, map_column: int, start_index_column: int, end_index_column: int=-1) -> bool:
        sig = inspect.signature(Mapper.add_binary_file)
        # Just put in the signature as a dictionary entry into the file_list
        self.file_list[file_name] = dict(sig.bind(map_column, start_index_column, end_index_column).arguments)
        self.file_list[file_name]["data_type"] = "binary"
        
        # Determine file dialect inferences
        return self._file_dialect_inference(file_name)

